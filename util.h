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



/**
 * chi-squared test is the sum of squared errors of a distribution
 * from expected distribution.
 */
double chi_squared_test(double *observed, double *expected, size_t len);

/**
 * count of occurrences of characters A-Z (ignoring case) in string
 * `str` of length `len`. Counts are returned in `freq` array. Return
 * value is total number of counted characters.
 */
int count_chars(unsigned char *str, size_t len, double freq[]);

/* convert integer counts in array `freq` (of length `len`) to
 * proportions of `total` count. in place. */
void count_to_freqs(double freq[], int total, size_t len);

/* return a score indicating likliehood that `str` is English
 *
 * based on frequency analysis, and χ-squared test. lower is better.
 */
double english_score(unsigned char *str, size_t len);

#endif /* CRYPTOPALS_UTIL_H */
