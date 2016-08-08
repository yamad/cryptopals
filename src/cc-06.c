#include "util.h"
#include "b64.h"
#include "xor.h"

#define MAXLEN 4096

int main(int argc, char *argv[])
{
	char ibuf[MAXLEN] = {0};    /* input buffer  */
	uint8_t obuf[MAXLEN] = {0}; /* output buffer */
	uint8_t key[MAXLEN]  = {0}; /* key buffer    */
	uint8_t buf[MAXLEN]  = {0}; /* byte buffer   */

	size_t rres = fread(ibuf, sizeof(char), MAXLEN, stdin);
	if (ferror(stdin)) {
		puts("Error reading from standard input\n");
		return -1;
	}

	int n  = b64_decode(ibuf, buf, b64_nbytes(ibuf));
	int ks = xor_break_repeatkey(buf, n, key, obuf);

	key[ks+1] = '\0';
	obuf[n+1] = '\0';
	printf("key: (%d) %s\n", ks, key);
	printf("%s\n", obuf);
	return 0;
}
