#include "util.h"

/* from Practical Cryptography */
/* http://practicalcryptography.com/cryptanalysis/letter-frequencies-various-languages/english-letter-frequencies/ */

/* English Letter Frequencies, A-Z  */
double ENG_LETTER_FREQ[26] = { 8.55, 1.60, 3.16, 3.87, 12.10, 2.18,	  \
                               2.09, 4.96, 7.33, 0.22, 0.81, 4.21,	  \
                               2.53, 7.17, 7.47, 2.07, 0.10, 6.33,	  \
                               6.73, 8.94, 2.68, 1.06, 1.83, 0.19,	  \
                               1.72, 0.11 };

/* note: 'ETAOIN SHRDLU' are the most frequent letters in English, in order */

double chi_squared_test(double *observed, double *expected, size_t len) {
	double sum = 0;
	for (int i = 0; i < len; i++) {
		sum += pow(observed[i] - expected[i], 2) / expected[i];
	}
	return sum;
}

int count_chars(unsigned char *str, size_t len, double freq[]) {
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

double english_score(unsigned char *str, size_t len) {
	double freq[26] = {0};
	int total = count_chars(str, len, freq);
	if (total == 0) return 2048;

	count_to_freqs(freq, total, 26);
	return chi_squared_test((double *)freq, ENG_LETTER_FREQ, 26);
}
