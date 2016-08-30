#include <stdio.h>

#include "util.h"
#include "aes.h"
#include "b64.h"

#define MAXLEN 4096

int main(int argc, char *argv[])
{
	ensure_argc(2, "AES-128 ECB decrypt -- Usage: %s <key>\n");

	char *key = argv[1];
	size_t keylen = strlen(key);

	char ibuf[MAXLEN] = {0};
	uint8_t bbuf[MAXLEN] = {0};
	uint8_t obuf[MAXLEN] = {0};

	int n;
	size_t c;
	while (!feof(stdin) || !ferror(stdin)) {
		if ((c = fread(ibuf, sizeof(uint8_t), MAXLEN, stdin)) == 0)
			break;
		if ((n = b64_decode(ibuf, bbuf, b64_nbytes(ibuf))) == 0)
			break;
		if ((n = aes128_ecb_decrypt(bbuf, n, (uint8_t *)key, keylen, obuf)) == 0)
			break;
		c = fwrite(obuf, sizeof(uint8_t), n, stdout);
	}
	return 0;
}
