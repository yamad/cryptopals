#ifndef CRYPTOPALS_XOR_H
#define CRYPTOPALS_XOR_H

/**
 * return in `dst` result of byte-for-byte XOR of `buf1` and `buf2`,
 * each with `len` bytes
 **/
int fixed_xor(unsigned char* buf1, unsigned char *buf2, size_t len, unsigned char* dst);

/**
 * return in `dst` XOR of `len` bytes of `buf` with `byte` (aka single-byte XOR)
 **/
int xor_on_byte(unsigned char* buf, unsigned char byte, size_t len, unsigned char* dst);

/**
 * brute-force guess key for `buf` encrypted with single-byte
 * XOR. return `key` and decrypted string in `out`
 */
void break_xor(unsigned char *buf, size_t len, unsigned char *key, unsigned char **out, double *score);

#endif /* CRYPTOPALS_XOR_H */
