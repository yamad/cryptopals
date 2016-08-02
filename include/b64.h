#ifndef CRYPTOPALS_B64_H
#define CRYPTOPALS_B64_H

#include <stdint.h>

unsigned char get_n_6bit(unsigned char *bytes, unsigned int n);
int b64_encode(unsigned char *bytes, int nbytes, char *b64);

/**
 * return value (0-63) of base-64 character (A-Za-z0-9+/)
 */
char from_b64(char b64_char);

/**
 * return number of bytes encoded by base-64 string `b64`
 */
int b64_nbytes(char *b64);

/**
 * Decode a base-64 group into bytes. Return number of bytes decoded.
 *
 * A group may be 2,3, or 4 characters, corresponding to 1, 2, or 3
 * bytes. Will not decode more than `maxbytes` bytes.
 */
int b64_decode_group(char *b64, uint8_t *bytes, size_t maxbytes);

/**
 *  Decode a base-64 string to bytes, up to `nbytes`. Return number of bytes decoded.
 */
int b64_decode(char *b64, uint8_t *bytes, size_t nbytes);

#endif /* CRYPTOPALS_B64_H */
