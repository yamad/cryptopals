#include <stdio.h>

#include "util.h"
#include "aes.h"
#include "b64.h"
#include "hex.h"

#define MAXLEN 5120

/* AES-128 CBC decrypt */
int main(int argc, char *argv[])
{
	ensure_argc(3, "AES-128 CBC decrypt -- Usage: %s <hex key> <hex iv> < <base-64 stdin>\n");

	char    base64[MAXLEN] = {0};
	uint8_t cipher[MAXLEN] = {0};
	uint8_t output[MAXLEN] = {0};

	uint8_t key[16] = {0};             /* AES-128 has a 128-bit key */
	uint8_t iv[AES_BLOCK_BYTES] = {0}; /* initialization vector */

	size_t keylen, ivlen, b64len, cipherlen, ptlen;
	keylen = hex_decode(argv[1], 16, key);
	ivlen  = hex_decode(argv[2], AES_BLOCK_BYTES, iv);

	b64len    = fread(base64, sizeof(uint8_t), MAXLEN, stdin);
	cipherlen = b64_decode(base64, cipher, b64_nbytes(base64));
	ptlen     = aes128_cbc_decrypt(cipher, cipherlen, key, keylen, iv, output);
	output[ptlen] = '\0';

	printf("%s\n", output);
	return 0;
}
