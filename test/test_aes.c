#include <stdlib.h>
#include <string.h>

#include "unity.h"

#include "aes.h"

void test_aes128_ecb_encrypt_decrypt(void)
{
	uint8_t *input = (uint8_t*)"CBC mode is a block cipher mode that allows us to encrypt irregularly-sized messages, despite the fact that a block cipher natively only transforms individual blocks.";
	uint8_t *key = (uint8_t*)"YELLOW SUBMARINE";

	int inlen = strlen((char*)input);
	int keylen = strlen((char*)key);

	int nblocks = (ceil(inlen/AES_BLOCK_BYTES)+1);

	uint8_t *cipher, *plaintext;
	cipher = (uint8_t *)malloc(sizeof(uint8_t) * nblocks*AES_BLOCK_BYTES);
	plaintext = (uint8_t *)malloc(sizeof(uint8_t) * nblocks*AES_BLOCK_BYTES);

	int elen, dlen;
	elen = aes128_ecb_encrypt(input, inlen, key, keylen, cipher);

	dlen = aes128_ecb_decrypt(cipher, elen, key, keylen, plaintext);
	plaintext[dlen] = '\0';

	TEST_ASSERT_EQUAL_INT(nblocks*AES_BLOCK_BYTES, elen);
	TEST_ASSERT_EQUAL_INT(inlen, dlen);

	TEST_ASSERT_EQUAL_STRING(input, plaintext);

    free(plaintext);
    free(cipher);
}


void test_aes128_cbc_encrypt_decrypt(void)
{
	uint8_t *input = (uint8_t*)"CBC mode is a block cipher mode that allows us to encrypt irregularly-sized messages, despite the fact that a block cipher natively only transforms individual blocks.";
	uint8_t *key = (uint8_t*)"YELLOW SUBMARINE";
	uint8_t iv[AES_BLOCK_BYTES]  = {0};

	int inlen = strlen((char*)input);
	int keylen = strlen((char*)key);

	int nblocks = (ceil(inlen/AES_BLOCK_BYTES)+1);

	uint8_t *cipher, *plaintext;
	cipher = (uint8_t *)malloc(sizeof(uint8_t) * nblocks*AES_BLOCK_BYTES);
	plaintext = (uint8_t *)malloc(sizeof(uint8_t) * nblocks*AES_BLOCK_BYTES);

	int elen, dlen;
	elen = aes128_cbc_encrypt(input, inlen, key, keylen, iv, cipher);

	dlen = aes128_cbc_decrypt(cipher, elen, key, keylen, iv, plaintext);
	plaintext[dlen] = '\0';

	TEST_ASSERT_EQUAL_INT(nblocks*AES_BLOCK_BYTES, elen);
	//TEST_ASSERT_EQUAL_INT(inlen, dlen);

	TEST_ASSERT_EQUAL_STRING(input, plaintext);
}
