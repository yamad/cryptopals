#include <stdint.h>
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
                       uint8_t* key, size_t keylen,
                       uint8_t* iv, uint8_t *out)
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
                       uint8_t* key, size_t keylen,
                       uint8_t* iv, uint8_t *out)
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
