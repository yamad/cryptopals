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

struct char_count {
	unsigned int letters[26];   /* A-Z */
	unsigned int space;           /* space characters */
	unsigned int punct;           /* punctuation */
	unsigned int nonprint;        /* non-printing characters */
	unsigned int extended;        /* extended ASCII characters */
};

void char_count_init(struct char_count *cc) {
	for (int i = 0; i < 26; i++)
		cc->letters[i] = 0;
	cc->space = 0;
	cc->punct = 0;
	cc->nonprint = 0;
	cc->extended = 0;
}

int count_chars(unsigned char *str, size_t len, struct char_count *nchars) {
	int c;                      /* current character */
	int nletters = 0;           /* letters counted */
	for (int i = 0; i < len && (c = *str++) != '\0'; i++) {
		if (isalpha(c)) {
			nchars->letters[(int)(toupper(c) - 'A')]++; /* assumes ASCII */
			nletters++;
		} else if (isspace(c)) {
			nchars->space++;
		} else if (ispunct(c)) {
			nchars->punct++;
		} else if (c > 126) {
			nchars->extended++;
		} else if (!isprint(c)) {
			nchars->nonprint++;
		}
	}
	return nletters;
}

void letter_freqs(struct char_count cc, int nletters, double freq[]) {
	for (int i = 0; i < 26; i++)
		freq[i] = cc.letters[i] / (double) nletters;
}

double english_score(unsigned char *str, size_t len) {
	struct char_count cc;
	char_count_init(&cc);

	int nletters = count_chars(str, len, &cc);
	if (nletters == 0) return INFINITY;

	double freq[26] = {0};
	letter_freqs(cc, nletters, freq);

	double score = chi_squared_test((double *)freq, ENG_LETTER_FREQ, 26);
	score += (cc.nonprint + cc.extended) * 10; /* punish non-letters */
	return score;
}
