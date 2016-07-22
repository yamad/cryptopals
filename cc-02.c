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
	int alen = decode_hex(argv[1], MAXLEN, abuf);
	int blen = decode_hex(argv[2], MAXLEN, bbuf);
	if (alen != blen)
		printf("Warn: strings are not equal length\n");

	int len = min(alen, blen);

	fixed_xor(abuf, bbuf, len, outbuf);
	encode_hex(outbuf, len, strbuf, MAXLEN*2+1);
	printf("%s\n", strbuf);
	return 0;
}
