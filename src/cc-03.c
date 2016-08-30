#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "util.h"
#include "xor.h"
#include "hex.h"

#define MAXBYTES 4096
int main(int argc, char *argv[])
{
	size_t nhex, nbyte;
	unsigned char *input;

	double score;
	unsigned char key, *out;

	ensure_argc(2, "1-byte XOR cipher -- Usage: %s <hex string>\n");
	nhex = strlen(argv[1]);

	/* decode hex string to bytes */
	nbyte = nhex / 2;
	input = malloc(sizeof(unsigned char) * nbyte);
	assert(input != NULL);

	hex_decode(argv[1], nbyte, input);
	xor_break_singlekey(input, nbyte, &key, &out, &score);

	printf("%d -- %s\n", key, out);

	free(out);
	free(input);
	return 0;
}
