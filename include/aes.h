#ifndef CRYPTOPALS_AES_H
#define CRYPTOPALS_AES_H

#include <stdint.h>

#include <openssl/aes.h>

#define AES_BLOCK_BYTES 16

enum encryption_mode { ECB_MODE, CBC_MODE };

int aes128_ecb_encrypt(uint8_t *plaintext, size_t len,
                       uint8_t *key, size_t keylen,
                       uint8_t *out);
int aes128_ecb_decrypt(uint8_t *cipher, size_t len,
                       uint8_t *key, size_t keylen,
                       uint8_t *out);

int aes128_cbc_encrypt(uint8_t *plaintext, size_t len,
                       uint8_t *key, size_t keylen,
                       uint8_t *iv, uint8_t *out);
int aes128_cbc_decrypt(uint8_t *cipher, size_t len,
                       uint8_t *key, size_t keylen,
                       uint8_t *iv, uint8_t *out);

/** returns 1 if ECB mode is detected in given cipher text. Otherwise, returns 0 */
int detect_ecb(uint8_t *cipher, size_t len);

int aes128_random_key(uint8_t *out);
int aes128_random_encrypt(uint8_t *plaintext, size_t len,
                          uint8_t *out, enum encryption_mode *mode);
int detect_aes128_encryption_mode(uint8_t *cipher, size_t len);

#endif /* CRYPTOPALS_AES_H */
