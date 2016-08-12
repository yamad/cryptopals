#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

/**
 * Pad block using PKCS#7 (pad with # of pad bytes)
 *
 * buffer `out` will take contents of `in`. extra bytes in `out` will
 * be filled with the count of extra bytes.
 *
 * @see RFC 2315, section 10.3, note 2.
 */
void pad_pkcs7(uint8_t *in, int inlen, uint8_t *out, int outlen)
{
	assert(inlen <= outlen);

	int pad = outlen - (inlen % outlen);
	memcpy(out, in, inlen);
	memset(out+inlen, pad, pad);
}

int main(int argc, char *argv[])
{
	ensure_argc(3, "Usage: %s <block size> <input string>\n");

	/* ingest arguments */
	int blksz = atoi(argv[1]);
	char *in = argv[2];
	if (blksz == 0) return -1;

	uint8_t *out = (uint8_t*)malloc(sizeof(uint8_t) * blksz);
	if (out == NULL) return -1;

	pad_pkcs7((uint8_t*)in, strlen(in), out, blksz);

	for (int i = 0; i < blksz; i++)
		printf("%.2x", out[i]);
	free(out);

	return 0;
}
