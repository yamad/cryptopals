#include <string.h>

#include "aes.h"

#define AES_BLOCK_BYTES 16

int aes128_ecb_decrypt(uint8_t *cipher, size_t len,
                       uint8_t* key, size_t keylen, uint8_t *out)
{
	/* OpenSSL's high-level EVP (envelope) API is supposedly simple */
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);

	/* padding is automatically handled */
	EVP_DecryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, key, NULL);

	int outlen;
	int ndecrypt = 0;

	/* decrypt each block */
	int nblks = len / AES_BLOCK_BYTES;
	for (int ib = 0; ib < nblks; ib++) {
		if (EVP_DecryptUpdate(&ctx, out+ndecrypt, &outlen,
		                      cipher, AES_BLOCK_BYTES) == -1)
			break;
		ndecrypt += outlen;
		cipher += AES_BLOCK_BYTES;
	}

	/* finish possible incomplete final block */
	if (EVP_DecryptFinal_ex(&ctx, out+ndecrypt, &outlen) != -1)
		ndecrypt += outlen;

	EVP_CIPHER_CTX_cleanup(&ctx);
	return ndecrypt;
}
