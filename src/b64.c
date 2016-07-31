#include <ctype.h>

#include "b64.h"

char B64_DIG[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

unsigned char get_n_6bit(unsigned char *bytes, unsigned int n)
{
    unsigned char x;
    int b = (n*6)/8;
    switch (n % 4) {
    case 0:
        x = (bytes[b] >> 2) & 0x3F;
        break;
    case 1:
        x = ((bytes[b] & 0x03) << 4) | ((bytes[b+1] & 0xF0) >> 4);
        break;
    case 2:
        x = ((bytes[b] & 0x0F) << 2) | ((bytes[b+1] & 0xC0) >> 6);
        break;
    case 3:
        x = (bytes[b] & 0x3F);
    }
    return x;
}

char from_b64(char b64_char) {
	if (isupper(b64_char)) {        /* uppercase letters */
		return (b64_char - 'A');
	} else if (islower(b64_char)) { /* lowercase letters */
		return (b64_char - 'a') + 26;
	} else if (isdigit(b64_char)) { /* digits */
		return (b64_char - '0') + 52;
	} else if (b64_char == '+')     /* plus (+) */
		return 62;
	else if (b64_char == '/')       /* backslash (/) */
		return 63;
	else if (b64_char == '=')       /* padding character */
		return 0;
	else                            /* unknown */
		return -1;
}

int encode_b64(unsigned char *bytes, int nbytes, char *b64)
{
    int i;                      /* hex byte counter (8-bit) */
    int e;                      /* base-64 byte counter (6-bit) */
    int extra = nbytes % 3;     /* extra bytes in hex */
    /* 4 base-b64 bytes for every 3 hex bytes */
    for (i=0; i<(nbytes/3); i++) {
        e = i*4;
        *b64++ = B64_DIG[get_n_6bit(bytes, e)];
        *b64++ = B64_DIG[get_n_6bit(bytes, e+1)];
        *b64++ = B64_DIG[get_n_6bit(bytes, e+2)];
        *b64++ = B64_DIG[get_n_6bit(bytes, e+3)];
    }
    /* pad extra bytes (if not divisible by 3) */
    e = i*4;
    switch (extra) {
    case 2:
        *b64++ = B64_DIG[get_n_6bit(bytes, e)];
        *b64++ = B64_DIG[get_n_6bit(bytes, e+1)];
        *b64++ = '=';
        *b64++ = '\0';
        break;
    case 1:
        *b64++ = B64_DIG[get_n_6bit(bytes, e)];
        *b64++ = B64_DIG[get_n_6bit(bytes, e+1)];
        *b64++ = '=';
        *b64++ = '=';
        *b64++ = '\0';
        break;
    default:
        break;
    }
    return e;
}
