#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CRYPTOPALS_UTIL_H
#define CRYPTOPALS_UTIL_H

/* inside main, make sure correct number of arguments `n` provided. if
 * not, print `msg` with binary name and quit */
#define ensure_argc(n, msg)	              \
	{ if (argc < n || argc > n) {		  \
			printf(msg, argv[0]);		  \
			return 1;					  \
		}								  \
	}

/** return minimum value */
int min(int a, int b);

/**
 * Utilites for character frequency analysis
 */

struct char_count;

/**
 * chi-squared test is the sum of squared errors of a distribution
 * from expected distribution.
 */
double chi_squared_test(double *observed, double *expected, size_t len);

/**
 * Count of occurrences of characters A-Z (ignoring case) in string
 * `str` of length `len`. Counts are returned in `freq` array. Return
 * value is total number of counted characters.
 */
int count_chars(unsigned char *str, size_t len, struct char_count *nchars);

/**
 * Convert letter counts in char_count `cc` to proportions of
 * `nletters` total count. return results in `freq`, assumed to be at
 * least 26 long. used for comparison to expected frequency distribution.
 */
void letter_freqs(struct char_count cc, int nletters, double freq[]);

/**
 * Return a score indicating likliehood that `str` is English
 *
 * based on frequency analysis, and χ-squared test. lower is better.
 */
double english_score(unsigned char *str, size_t len);

/**
 * Count 1-bits in byte `x`
 */
int count_bits(uint8_t x);

/**
 * return Hamming distance (# of differing bits) between byte arrays
 */
int hamming_distance(uint8_t *a, uint8_t *b, size_t maxlen);

int transpose_blocks(uint8_t *b, size_t blen, size_t blocklen, uint8_t ***t);

int fill_block(uint8_t *buf, size_t len, size_t stride, size_t offset, uint8_t *dst);

void fill_random_bytes(uint8_t *buf, size_t len);

int randto(int max);

/**
 * Pad block using PKCS#7 (pad with # of pad bytes). Return # of pad bytes
 *
 * buffer `out` will take contents of `in`. extra bytes in `out` will
 * be filled with the count of extra bytes. the length of out must be
 * 2x the `outlen`, because if `inlen` == `outlen` then `outlen` bytes
 * are added as a pad.
 *
 * @see RFC 2315, section 10.3, note 2.
 */
int pad_pkcs7(uint8_t *in, int inlen, uint8_t *out, int outlen);

#endif /* CRYPTOPALS_UTIL_H */
