#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "xor.h"
#include "hex.h"


#define MAXLEN 2048
#define HEXLEN 4096

/* repeating-key XOR */
int main(int argc, char *argv[]) {
	ensure_argc(2, "Usage: %s <key>\n");

	char ibuf[MAXLEN] = {0};    /* input buffer */
	char obuf[MAXLEN] = {0};    /* output buffer */
	char hbuf[HEXLEN] = {0};    /* hex string buffer */

	/* name arguments */
	char *key = argv[1];
	int keylen = strlen(key);
	int keyoff = 0;

	int len;
	/* read stdin, XOR as it goes */
	while(fgets(ibuf, MAXLEN, stdin) != NULL) {
		len = strlen(ibuf);

		/* XOR with given key, output as hex string */
		xor_repeatkey((unsigned char *)ibuf, len,
			(unsigned char *)key, keylen, keyoff, (unsigned char *)obuf);
		obuf[len+1] = '\0';
		hex_encode((unsigned char *)obuf, len, hbuf, HEXLEN);
		printf("%s", hbuf);

		keyoff = (len+keyoff) % keylen;
	}
	printf("\n");
	return 0;
}
