#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#include "util.h"
#include "xor.h"

int fixed_xor(unsigned char* buf1, unsigned char *buf2, size_t len, unsigned char* dst) {
	int n;
	for (n=0; n < len; n++) {
		dst[n] = (unsigned char)(buf1[n] ^ buf2[n]);
	}
	return n;
}

int xor_on_byte(unsigned char* buf, unsigned char byte, size_t len, unsigned char* dst) {
	int n;
	for (n=0; n < len; n++) {
		dst[n] = (unsigned char)(buf[n] ^ byte);
	}

	return n;
}

void break_xor(unsigned char *buf, size_t len,
	unsigned char *key, unsigned char **out, double *score) {

	double curscore;
	unsigned char curkey = 0;
	unsigned char *curout = malloc(sizeof(char) * (len+1));
	assert(curout != NULL);
	curout[len+1] = '\0';

	*score = INFINITY;
	*key = 0;
	*out = malloc(sizeof(char) * (len+1));
	assert(*out != NULL);

	for (curkey = 0; curkey < 255; curkey++) {
		/* try candidate single-byte XOR and score */
		xor_on_byte(buf, curkey, len, curout);
		curscore = english_score(curout, len);

		/* save minimum score */
		if (curscore < *score) {
			*score = curscore;
			*key = curkey;
			memcpy(*out, curout, len+1);
		}
	}
	free(curout);
}
