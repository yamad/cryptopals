#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#include "util.h"
#include "xor.c"
#include "hex_to_b64.c"

/* from Practical Cryptography */
/* http://practicalcryptography.com/cryptanalysis/letter-frequencies-various-languages/english-letter-frequencies/ */

/* English Letter Frequencies, A-Z  */
double ENG_LETTER_FREQ[26] = { 8.55, 1.60, 3.16, 3.87, 12.10, 2.18, 2.09, 4.96, 7.33, 0.22, 0.81, \
                               4.21, 2.53, 7.17, 7.47, 2.07, 0.10, 6.33, 6.73, 8.94, 2.68, 1.06, 1.83, 0.19, 1.72, 0.11 };

/* note: 'ETAOIN SHRDLU' are the most frequent letters in English, in order */

/* chi-squared test is the sum of squared errors of a distribution
 * from expected distribution. */
double chi_squared_test(double *observed, double *expected, size_t len) {
	double sum = 0;
	for (int i = 0; i < len; i++) {
		sum += pow(observed[i] - expected[i], 2) / expected[i];
	}
	return sum;
}

int count_chars(char *str, size_t len, double freq[]) {
	int c;                      /* current character */
	int n = 0;                  /* characters counted */
	for (int i = 0; i < len && (c = *str++) != '\0'; i++) {
		if (!isalpha(c)) continue;
		freq[(int)(toupper(c) - 'A')]++;
		n++;
	}
	return n;
}

void count_to_freqs(double freq[], int total, size_t len) {
	for (int i = 0; i < len; i++)
		freq[i] /= total;
}

double english_score(char *str, size_t len) {
	double freq[26] = {0};
	int total = count_chars(str, len, freq);
	if (total == 0) return 2048;

	count_to_freqs(freq, total, 26);
	return chi_squared_test((double *)freq, ENG_LETTER_FREQ, 26);
}

void byte_fill(unsigned char byte, size_t len, unsigned char *buf) {
	int i = 0;
	while (i++ < len)
		*buf++ = byte;
}

void decode_single_byte_xor(unsigned char *in, size_t len, unsigned char key, unsigned char *keybuf, unsigned char *out) {
	byte_fill(key, len, keybuf);
	fixed_xor(in, keybuf, len, out);
}

#define MAXBYTES 4096
int main(int argc, char *argv[])
{
	ensure_argc(2, "Usage: %s <hex string>\n");
	size_t nhex = strlen(argv[1]);

	/* decode hex string to bytes */
	size_t nbyte = nhex / 2;
	unsigned char *inbytes = malloc(sizeof(unsigned char) * nbyte);
	assert(inbytes != NULL);
	decode_hex(argv[1], nbyte, inbytes);

	unsigned char *keybuf = malloc(sizeof(unsigned char) * nbyte);
	assert(keybuf != NULL);

	double curscore;
	unsigned char curkey = 0;
	unsigned char *curout = malloc(sizeof(char) * (nbyte+1));
	assert(curout != NULL);
	curout[nbyte+1] = '\0';

	double bestscore = INFINITY;
	unsigned char bestkey = 0;
	unsigned char *bestout = malloc(sizeof(char) * (nbyte+1));
	assert(bestout != NULL);

	for (curkey = 0; curkey < 255; curkey++) {
		decode_single_byte_xor(inbytes, nbyte, curkey, keybuf, curout);
		curscore = english_score(curout, nbyte);
		if (curscore < bestscore) {
			bestscore = curscore;
			bestkey = curkey;
			memcpy(bestout, curout, nbyte+1);

			printf("%d\n", bestkey);
			printf("%s\n", bestout);
		}
	}

	free(bestout);
	free(curout);
	free(keybuf);
	free(inbytes);
	return 0;
}
