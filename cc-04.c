#include <stdio.h>
#include <stdlib.h>

#include "xor.h"
#include "hex.h"
#include "util.h"

int main(int argc, char *argv[])
{
	ensure_argc(2, "Usage: %s <filename>\n");

	FILE *f = fopen(argv[1], "r");
	if (!f) return -1;

	double score, bestscore = INFINITY;
	unsigned char key, bestkey;
	unsigned char *out, *bestout;
	int bestline;

	int len;
	char buf[1024];
	unsigned char bytes[1024];
	int i = 0;
	while (fgets(buf, sizeof(buf), f) != NULL) {
		len = strlen(buf);
		decode_hex(buf, len, bytes);
		break_xor(bytes, len/2, &key, &out, &score);

		if (score < bestscore) {
			bestscore = score;
			bestkey = key;
			bestout = out;
			bestline = i;
		}
//		printf("%d, %d, %.4f -- %s\n", i, key, score, out);
		i++;
	}

	printf("%d, %d -- %s\n", bestline, bestkey, bestout);

	fclose(f);
	return 0;
}
