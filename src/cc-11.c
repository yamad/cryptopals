#include <stdio.h>
#include <time.h>

#include "util.h"
#include "aes.h"

#define MAXLEN 5120

/* randomly encrypt and detect mode */
int main(int argc, char *argv[])
{
	ensure_argc(1, "ECB/CBC detection oracle -- Usage: %s <encrypt stdin> \n");

	srand(time(0));             /* seed with current time, unsafe but simple */

	char plaintext[MAXLEN] = {0};
	uint8_t cipher[MAXLEN] = {0};
	uint8_t output[MAXLEN] = {0};

	enum encryption_mode mode, predict_mode;
	size_t ptlen, clen;

	ptlen = fread(plaintext, sizeof(uint8_t), MAXLEN, stdin);

	for (int i=0; i < 10; i++) {
		clen = aes128_random_encrypt((uint8_t*)plaintext, ptlen, cipher, &mode);
		predict_mode = aes128_encryption_oracle(cipher, clen);
		if (mode != predict_mode) {
			printf("Prediction failure. Was %d, but predicted %d\n", mode, predict_mode);
			return -1;
		}
	}

	printf("Successfully detected 10 random encryption modes\n");
	return 0;
}
