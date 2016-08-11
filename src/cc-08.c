#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "util.h"
#include "hex.h"

#define ENTRY_BYTES 160        /* hardcode input length, keep it simple */
#define AES_BLOCK_BYTES 16     /* # of bytes in an AES block (128 bits) */

/**
 * score to test if `input` bytes were encrpyted by AES in ECB mode
 * (lower is better)
 *
 * Gives the average hamming distance between all pairs of AES
 * blocks. Because the same input results in identical output in ECB
 * mode, the hamming distance between such blocks will be zero.
 */
double score_ecb(uint8_t *input, int len)
{
	double total = 0;
	int n = 0;
	int nblks = len / AES_BLOCK_BYTES;
	for (int ib = 0; ib < nblks; ib++) {
		for (int jb = ib+1; jb < nblks; jb++) {
			total += hamming_distance(input+(AES_BLOCK_BYTES*ib),
			                          input+(AES_BLOCK_BYTES*jb), AES_BLOCK_BYTES);
			n++;
		}
	}
	return total / n;
}


int main(int argc, char *argv[])
{
	ensure_argc(2, "Usage: %s <input file>\n");

	char str[ENTRY_BYTES*2+2] = {0};
	uint8_t hex[ENTRY_BYTES] = {0};
	uint8_t best_hex[ENTRY_BYTES] = {0};

	double score, best_score = INFINITY;
	int best_index;

	FILE *fp = fopen(argv[1], "r");

	/* run through input lines, find lowest score */
	int line = 0;
	while (fgets(str, ENTRY_BYTES*2+2, fp) != NULL) {
		hex_decode(str, ENTRY_BYTES, hex);
		score = score_ecb(hex, ENTRY_BYTES);
		if (score < best_score) {
			best_score = score;
			best_index = line;
			memcpy(best_hex, hex, ENTRY_BYTES);
		}
		line++;
	}

	/* display result */
	printf("%d ", best_index);
	for (int i = 0; i < ENTRY_BYTES; i++)
		printf("%x", best_hex[i]);

	fclose(fp);
	return 0;
}
