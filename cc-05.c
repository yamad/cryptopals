#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "xor.h"
#include "hex.h"

/* repeating-key XOR */
int main(int argc, char *argv[]) {
	ensure_argc(3, "Usage: %s <key> <input string>\n");

	/* name arguments */
	char *key = argv[1];
	char *input = argv[2];

	/* calculate buffer lengths */
	int len = strlen(input);
	int keylen = strlen(key);
	int hexlen = len*2 + 1;

	/* allocate buffers */
	unsigned char *bytes = malloc(sizeof(unsigned char) * len);
	char *hexout = malloc(sizeof(char) * hexlen);

	/* quit if memory allocation fails */
	if (!bytes) return -1;
	if (!hexout) return -1;

	/* XOR with given key, output as hex string */
	repeatkey_xor((unsigned char *)input, len, (unsigned char *)key, keylen, bytes);
	encode_hex((unsigned char *)bytes, len, hexout, hexlen);
	printf("%s\n", hexout);

	free(hexout);
	free(bytes);
	return 0;
}
