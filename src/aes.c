#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "util.h"
#include "aes.h"

#define AES_BLOCK_BYTES 16

int aes128_ecb_decrypt(uint8_t *cipher, size_t len,
                       uint8_t* key, size_t keylen, uint8_t *out)
{
	assert(len % AES_BLOCK_BYTES == 0); /* properly encrypted AES
	                                     * should always results in
	                                     * perfect multiple of block
	                                     * size */

	AES_KEY akey;
	AES_set_decrypt_key(key, keylen*CHAR_BIT, &akey);

	/* decrypt each block */
	int nblks = len / AES_BLOCK_BYTES;
	for (int ib = 0; ib < nblks; ib++) {
		AES_decrypt(cipher, out, &akey);
		cipher += AES_BLOCK_BYTES;
		out += AES_BLOCK_BYTES;
	}

	/* unpad, return length minus pad bytes */
	return len - *(out-1);
}
