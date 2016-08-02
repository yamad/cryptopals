#include "util.h"
#include "xor.h"
#include "hex.h"

#define MAXLEN 1024

static int min(int a, int b) {
	if (a > b)
		return b;
	return a;
}

int main(int argc, char *argv[])
{
	unsigned char abuf[MAXLEN] = {0};
	unsigned char bbuf[MAXLEN] = {0};
	unsigned char outbuf[MAXLEN] = {0};
	char strbuf[MAXLEN*2 + 1] = {0};

	ensure_argc(3, "Usage: %s <hex1> <hex2>");
	int alen = hex_decode(argv[1], MAXLEN, abuf);
	int blen = hex_decode(argv[2], MAXLEN, bbuf);
	if (alen != blen)
		printf("Warn: strings are not equal length\n");

	int len = min(alen, blen);

	xor_fixed(abuf, bbuf, len, outbuf);
	hex_encode(outbuf, len, strbuf, MAXLEN*2+1);
	printf("%s\n", strbuf);
	return 0;
}
