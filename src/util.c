#include "util.h"

int min(int a, int b) {
	if (a > b)
		return b;
	return a;
}

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
	for (int i = 0; i < len && (c = *str++); i++) {
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

int count_bits(uint8_t x)
{
#define ON(n) ((x & (1 << n)) == (1 << n) ? 1 : 0 )
	return ON(7) + ON(6) + ON(5) + ON(4) + ON(3) + ON(2) + ON(1) + ON(0);
#undef ON
	/* unsigned n; */
	/* n = (x >> 1) & 033333333333;       // Count bits in */
	/* x = x - n;                         // each 3-bit */
	/* n = (n >> 1) & 033333333333;       // field. */
	/* x = x - n; */
	/* x = (x + (x >> 3)) & 030707070707; // 6-bit sums. */
	/* return x%63;                       // Add 6-bit sums. */
}

int hamming_distance(uint8_t *a, uint8_t *b, size_t len)
{
	int dist = 0;
	for (int i = 0; i < len; i++)
		dist += count_bits((uint8_t) a[i] ^ b[i]); /* count XOR bits */
	return dist;
}

int transpose_blocks(uint8_t *b, size_t blen, size_t blocklen, uint8_t ***t)
{
	/* setup `blocklen` blocks of size `nblocks` */
	int nblocks = blen / blocklen;
	*t = (uint8_t **)malloc(sizeof(uint8_t*) * blocklen);
	for (int i = 0; i < blocklen; i++) {
		(*t)[i] = (uint8_t *)calloc(nblocks+1, sizeof(uint8_t));
		if ((*t)[i] == NULL) return -1;
		(*t)[i][nblocks+1] = '\0';
	}

	/* transfer bytes, first byte in each block in first row, etc. */
	int r, c, i;
	for (r = 0; r < blocklen; r++) {
		for (i = r, c = 0; i < blen && c < nblocks; i+=blocklen, c++) {
			(*t)[r][c] = b[i];
		}
	}

	return 0;
}

int fill_block(uint8_t *buf, size_t len, size_t stride, size_t offset, uint8_t *dst)
{
	int n = 0;
	for (int i = offset; i < len; i+=stride) {
		dst[n++] = buf[i];
	}
	return n;
}

int pad_pkcs7(uint8_t *in, int inlen, uint8_t *out, int outlen)
{
	assert(inlen <= outlen);

	int npad = outlen - (inlen % outlen);
	memcpy(out, in, inlen);
	memset(out+inlen, npad, npad);
	return npad;
}

int randto(int max)
{
	return rand() / (RAND_MAX / (max+1));
}

void fill_random_bytes(uint8_t *buf, size_t len)
{
	for (int i = 0; i < len; i++)
		*buf++ = randto(255);
}
