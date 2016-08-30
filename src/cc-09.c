#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

int main(int argc, char *argv[])
{
	ensure_argc(3, "Pad PKCS#7 -- Usage: %s <block size> <input string>\n");

	/* ingest arguments */
	int blksz = atoi(argv[1]);
	char *in = argv[2];
	if (blksz == 0) return -1;

	uint8_t *out = (uint8_t*)malloc(sizeof(uint8_t) * (blksz*2));
	if (out == NULL) return -1;

	int len = strlen(in);
	int nb  = pad_pkcs7((uint8_t*)in, len, out, blksz);

	for (int i = 0; i < len+nb; i++)
		printf("%.2x", out[i]);
	free(out);

	return 0;
}
