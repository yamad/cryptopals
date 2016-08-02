#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

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

int b64_encode(unsigned char *bytes, int nbytes, char *b64)
{
	int i;                  /* hex byte counter (8-bit) */
	int e;                  /* base-64 byte counter (6-bit) */
	int extra = nbytes % 3; /* extra bytes in hex */
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

int b64_decode(char *b64, uint8_t *bytes, size_t nbytes)
{
	int i = nbytes;         /* bytes remaining */
	for (; i > 0; b64 += 4, bytes += 3)
		i -= b64_decode_group(b64, bytes, i);
	return nbytes - i;      /* # of decoded bytes */
}

int b64_nbytes(char *b64)
{
	/* count base-64 characters, without padding */
	int b64len = 0;
	char b;
	while ((b = *b64++) && b != '\0' && b != '=')
		b64len++;

	int nbytes = (b64len / 4) * 3; /* full 4-char base-64 groups*/
	switch (b64len % 4) {          /* last group may be 2 or 3 chars */
	case 3:
		return nbytes + 2;
	case 2:
		return nbytes + 1;
	default:
		return nbytes;
	}
}

int b64_decode_group(char *b64, uint8_t *bytes, size_t maxbytes)
{
	/* get base-64 value and mask low-order 6 bits */
	uint8_t b1, b2, b3, b4;
	b1 = from_b64(*b64++) & 0x3F;
	b2 = from_b64(*b64++) & 0x3F;
	*bytes++ = (b1 << 2) | (b2 >> 4);
	if (maxbytes <= 1 || *b64 == '\0' || *b64 == '=')
		return 1; /* only 1 byte in group */

	b3 = from_b64(*b64++) & 0x3F;
	*bytes++ = (b2 << 4) | (b3 >> 2);
	if (maxbytes <= 2 || *b64 == '\0' || *b64 == '=')
		return 2; /* only 2 bytes in group */

	b4 = from_b64(*b64)   & 0x3F;
	*bytes = (b3 << 6) | (b4 >> 0);
	return 3;
}
