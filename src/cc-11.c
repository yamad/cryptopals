#include <stdio.h>
#include <time.h>

#include "util.h"
#include "aes.h"

#define MAXLEN 5120
#define TESTLEN 2048

/* randomly encrypt and detect mode */
int main(int argc, char *argv[])
{
	srand(time(0));             /* seed with current time, unsafe but simple */

	char plaintext[TESTLEN];
    memset(plaintext, 'A', TESTLEN);

	uint8_t cipher[MAXLEN] = {0};

	enum encryption_mode mode, predict_mode;
	size_t ptlen, clen;

	for (int i=0; i < 10; i++) {
		clen = aes128_random_encrypt((uint8_t*)plaintext, TESTLEN, cipher, &mode);
		predict_mode = detect_aes128_encryption_mode(cipher, clen);

        if (mode != predict_mode) {
			printf("Prediction failure. Was %d, but predicted %d\n", mode, predict_mode);
			return -1;
		}
	}

	printf("Successfully detected 10 random encryption modes\n");
	return 0;
}
