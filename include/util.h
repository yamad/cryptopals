#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

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

#endif /* CRYPTOPALS_UTIL_H */
