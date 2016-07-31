#include <string.h>
#include "hex.h"

char HEX_DIGU[] = "0123456789ABCDEF";
char HEX_DIGL[] = "0123456789abcdef";

int decode_hex(char *hex, int nbytes, unsigned char *bytes)
{
	int a;
	int n = 0;
	while ((a = get_hex_byte(hex)) != -1 && n < nbytes) {
		bytes[n++] = a;
		hex += 2;
	}
	return n;
}

int encode_hex(unsigned char *bytes, int nbytes, char *hex, int nhex) {
	int b;
	int i, j;
	for (i = j = 0; i < nbytes && j < nhex-1;) {
		b = bytes[i++];
		hex[j++] = HEX_DIGL[((b & 0xF0) >> 4)];
		hex[j++] = HEX_DIGL[((b & 0x0F) >> 0)];
	}
	hex[j+1] = '\0';
	return j;
}

short int get_hex_byte(char *in)
{
    short int a, c, va;
    char *v;
    int i;
    i = c = 0;
    while ((a = *in++) != '\0' && i++<2) {
        /* find value of hex digit */
        if ((v = (char*)memchr(HEX_DIGU, a, sizeof(HEX_DIGU))) != NULL) {
            va = v - HEX_DIGU;
        } else if ((v = (char*)memchr(HEX_DIGL, a, sizeof(HEX_DIGL))) != NULL) {
            va = v - HEX_DIGL;
        } else {                /* invalid hex digit */
            return -1;
        }
        c = c * 16 + va;
    }
    if (i < 2) return -1;
    return c;
}
