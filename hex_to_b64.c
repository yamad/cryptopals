#include <stdio.h>
#include <string.h>

short int get_hex_byte(char *in);
unsigned char get_n_6bit(unsigned char *bytes, unsigned int n);
int encode_b64(unsigned char *bytes, int nbytes, char *b64);

char HEX_DIGU[] = "0123456789ABCDEF";
char HEX_DIGL[] = "0123456789abcdef";
char B64_DIG[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define MAXBYTE 1000

int main(int argc, char *argv[])
{
    unsigned char buf[MAXBYTE] = {0};
    char b64_buf[MAXBYTE] = {0};
    int a;

    if (argc < 2 || argc > 3) {
       printf("Usage: %s <hex value>\n", argv[0]);
       return 1;
    }
    char* s = argv[1];          /* get input hex number */

    /* convert hex string to bytes */
    int n = 0;
    while ((a = get_hex_byte(s)) != -1 && n<1000) {
        buf[n++] = a;
        s += 2;
    }
    if (n == 1000) printf("Too many bytes.\n");

    int e = encode_b64(buf, n, b64_buf);
    printf("%s\n", b64_buf);
    return 0;
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
