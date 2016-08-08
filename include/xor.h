#ifndef CRYPTOPALS_XOR_H
#define CRYPTOPALS_XOR_H

#include <stdint.h>

/**
 * return in `dst` result of byte-for-byte XOR of `buf1` and `buf2`,
 * each with `len` bytes
 **/
int xor_fixed(unsigned char* buf1, unsigned char *buf2, size_t len, unsigned char* dst);

/**
 * return in `dst` XOR of `len` bytes of `buf` with `byte` (aka single-byte XOR)
 **/
void xor_singlekey(unsigned char* buf, unsigned char byte,
                   size_t len, unsigned char* dst);

/**
 * encrypt `buf` with `key` using repeating-key XOR. return result in `dst`
 *
 * @param len    size of buffer to encrypt and destination buffer
 * @param keylen size of key
 * @param keyoff use key at offset
 **/
void xor_repeatkey(unsigned char *buf, size_t len,
                   unsigned char *key, size_t keylen, int keyoff,
                   unsigned char *dst);

/**
 * brute-force guess key for `buf` encrypted with single-byte
 * XOR. return `key` and decrypted string in `out`
 */
void xor_break_singlekey(unsigned char *buf, size_t len,
                         unsigned char *key, unsigned char **out, double *score);

int xor_break_repeatkey(uint8_t *buf, size_t len, uint8_t *key, uint8_t *out);

int guess_best_keysize(uint8_t *buf, size_t len, double *score);
double score_keysize(uint8_t *buf, size_t len, int keysize);

#endif /* CRYPTOPALS_XOR_H */
