#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "util.h"
#include "aes.h"
#include "xor.h"

int aes128_ecb_encrypt(uint8_t *plaintext, size_t len,
                       uint8_t *key, size_t keylen, uint8_t *out)
{
	uint8_t buf[AES_BLOCK_BYTES*2] = {0};

	AES_KEY akey;
	AES_set_encrypt_key(key, keylen*CHAR_BIT, &akey);

	/* encrypt each block */
	size_t ip = 0;
	int nblocks = len / AES_BLOCK_BYTES;
	int nextra  = len % AES_BLOCK_BYTES;

	for (int i = 0; i < nblocks; i++) {
		AES_encrypt(plaintext+ip, out+ip, &akey);
		ip += AES_BLOCK_BYTES;
	}

	/* pad last block */
	/* if last block is full, `out` is in place to fill a full pad block */
	int npad = pad_pkcs7(plaintext+ip, nextra, buf, AES_BLOCK_BYTES);
	AES_encrypt(buf, out+ip, &akey);
	return len + npad;
}

int aes128_ecb_decrypt(uint8_t *cipher, size_t len,
                       uint8_t *key, size_t keylen, uint8_t *out)
{
	assert(len % AES_BLOCK_BYTES == 0); /* properly encrypted AES
	                                     * should always result in
	                                     * perfect multiple of block
	                                     * size */

	AES_KEY akey;
	AES_set_decrypt_key(key, keylen*CHAR_BIT, &akey);

	/* decrypt each block */
	size_t ip = 0;
	for (; ip < len; ip += AES_BLOCK_BYTES)
		AES_decrypt(cipher+ip, out+ip, &akey);

	/* unpad, return length minus pad bytes */
	return len - *(out+ip-1);
}

int aes128_cbc_encrypt(uint8_t *plaintext, size_t len,
                       uint8_t *key, size_t keylen,
                       uint8_t *iv, uint8_t *out)
{
	uint8_t buf[AES_BLOCK_BYTES*2] = {0}; /* 2x block for pad */

	AES_KEY akey;
	AES_set_encrypt_key(key, keylen*CHAR_BIT, &akey);


	uint8_t *xp = iv;           /* block to XOR with */
	size_t ip = 0;              /* bytes processed */
	int nblocks = len / AES_BLOCK_BYTES;
	int nextra  = len % AES_BLOCK_BYTES;

	/* encrypt each block */
	for (int i = 0; i < nblocks; i++) {
		xor_fixed(plaintext+ip, xp, AES_BLOCK_BYTES, buf);
		AES_encrypt(buf, out+ip, &akey);
		xp = out+ip;
		ip += AES_BLOCK_BYTES;
	}

	/* pad last block */
	/* if last block is full, `out` is in place to fill a full pad block */
	int npad = pad_pkcs7(plaintext+ip, nextra, buf, AES_BLOCK_BYTES);
	xor_fixed(buf, xp, AES_BLOCK_BYTES, buf);
	AES_encrypt(buf, out+ip, &akey);
	return len + npad;
}

int aes128_cbc_decrypt(uint8_t *cipher, size_t len,
                       uint8_t *key, size_t keylen,
                       uint8_t *iv, uint8_t *out)
{
	uint8_t buf[AES_BLOCK_BYTES*2] = {0}; /* 2x block for pad */

	AES_KEY akey;
	AES_set_decrypt_key(key, keylen*CHAR_BIT, &akey);


	uint8_t *xp = iv;           /* block to XOR with */
	size_t ip = 0;              /* bytes processed */

	/* decrypt each block */
	for (; ip < len; ip += AES_BLOCK_BYTES) {
		AES_decrypt(cipher+ip, buf, &akey);
		xor_fixed(buf, xp, AES_BLOCK_BYTES, out+ip);
		xp = cipher+ip;
	}

	/* unpad, return length minus pad bytes */
	return len - *(out+ip-1);
}

int aes128_random_key(uint8_t *out)
{
    fill_random_bytes(out, AES_BLOCK_BYTES);
    return AES_BLOCK_BYTES;
}

int aes128_random_encrypt(uint8_t *plaintext, size_t len,
                          uint8_t *out, enum encryption_mode *mode)
{
	/* append 5-10 random bytes each to front and back of plaintext */
	int nlead = randto(5) + 5;
	int ntail = randto(5) + 5;

    int inlen = len + nlead + ntail;
    uint8_t *input = malloc(inlen * sizeof(uint8_t));
	if (input == NULL) return -1;

	fill_random_bytes(input, nlead);
	memcpy(input+nlead, plaintext, len);
	fill_random_bytes(input+len+nlead, ntail);

    int outlen;
	uint8_t key[AES_BLOCK_BYTES];
	uint8_t iv[AES_BLOCK_BYTES];
    aes128_random_key(key);

	*mode = randto(1);          /* randomly choose encryption mode */
	switch (*mode) {
	case ECB_MODE:              /* ECB */
		outlen = aes128_ecb_encrypt(input, inlen, key, AES_BLOCK_BYTES, out);
        break;
	default:                    /* CBC */
		fill_random_bytes(iv, AES_BLOCK_BYTES);
		outlen = aes128_cbc_encrypt(input, inlen, key, AES_BLOCK_BYTES, iv, out);
	}

    free(input);
    return outlen;
}

int detect_aes128_encryption_mode(uint8_t *cipher, size_t len)
{
	for (int off=0; off < AES_BLOCK_BYTES; off++)
		if (detect_ecb(cipher+off, len-off))
			return ECB_MODE;
	return CBC_MODE;
}

/**
 * Return true (1) if bytes in `cipher` were encrypted by AES in ECB mode
 *
 * Detects if there are identical blocks in `cipher`
 */
int	detect_ecb(uint8_t *cipher, size_t len)
{
	int nblks = len / AES_BLOCK_BYTES;
	for (int ib = 0; ib < nblks-1; ib++) {
        uint8_t *blk1 = cipher+(AES_BLOCK_BYTES*ib);

        for (int jb = ib+1; jb < nblks; jb++) {
            uint8_t *blk2 = cipher+(AES_BLOCK_BYTES*jb);
            if (hamming_distance(blk1, blk2, AES_BLOCK_BYTES) == 0)
                return 1;
        }
    }
	return 0;
}
