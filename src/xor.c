#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "util.h"
#include "xor.h"

int xor_fixed(unsigned char *buf1, unsigned char *buf2, size_t len,
              unsigned char *dst)
{
	int n;
	for (n = 0; n < len; n++) {
		dst[n] = (unsigned char)(buf1[n] ^ buf2[n]);
	}
	return n;
}

void xor_singlekey(unsigned char *buf, unsigned char byte, size_t len,
                  unsigned char *dst)
{
	xor_repeatkey(buf, len, &byte, 1, 0, dst);
}

void xor_repeatkey(unsigned char *buf, size_t len,
                   unsigned char *key, size_t keylen,
                   int keyoff, unsigned char *dst)
{
	for (int i = 0; i < len; i++) {
		dst[i] = (unsigned char)(buf[i] ^ key[(i + keyoff) % keylen]);
	}
}

void xor_break_singlekey(unsigned char *buf, size_t len,
                         unsigned char *key, unsigned char **out, double *score)
{
	double curscore;
	unsigned char curkey = 0;
	unsigned char *curout = malloc(sizeof(char) * (len+1));
	assert(curout != NULL);
	curout[len+1] = '\0';

	*score = INFINITY;
	*key = 0;
	*out = malloc(sizeof(char) * (len + 1));
	assert(*out != NULL);

	for (curkey = 0; curkey < 255; curkey++) {
		/* try candidate single-byte XOR and score */
		xor_singlekey(buf, curkey, len, curout);
		curscore = english_score(curout, len);

		/* save minimum score */
		if (curscore < *score) {
			*score = curscore;
			*key = curkey;
			memcpy(*out, curout, len + 1);
		}
	}
	free(curout);
}

#define MINKEYSIZE 2
#define MAXKEYSIZE 40

double score_keysize(uint8_t *buf, size_t len, int keysize)
{
	assert(keysize > 0);
	assert(len > 0);

	int nblocks = len / keysize;

	/* average edit distances between all pairs of (at most) 4 blocks */
	int n = 0;
	double total = 0;
	for (int i = 0; i < min(4, nblocks); i++) {
		for (int j = i+1; j < min(4, nblocks); j++) {
			int d = hamming_distance(buf+(keysize*i),
			                         buf+(keysize*j), keysize);
			total += d;
			n++;
		}
	}
	return total / (keysize * n);
}

int guess_best_keysize(uint8_t *buf, size_t len, double *score)
{
	int bestks = MINKEYSIZE;
	double bestd  = INFINITY;

	int ks;
	double d;
	for (ks = MINKEYSIZE; ks <= MAXKEYSIZE; ks++) {
		d = score_keysize(buf, len, ks);
		if (d < bestd) {
			bestd  = d;
			bestks = ks;
		}
	}
	*score = bestd;
	return bestks;
}

int xor_break_repeatkey(uint8_t *buf, size_t len, uint8_t *key, uint8_t *plaintext)
{
	double keyscore;
	int keysize;

	double dumscore;

	uint8_t *block, *dcrypt;
	block = (uint8_t *)malloc(sizeof(uint8_t) * (len+1));

	keysize = guess_best_keysize(buf, len, &keyscore);

	int n;
	for (int i = 0; i < keysize; i++) {
		n = fill_block(buf, len, keysize, i, block);
		xor_break_singlekey(block, n, &(key[i]), &dcrypt, &dumscore);

		/* distribute decrypted characters into output */
		for (int j = 0; j < n; j++)
			plaintext[i+(keysize*j)] = dcrypt[j];
		free(dcrypt);
	}

	free(block);
	return keysize;
}
