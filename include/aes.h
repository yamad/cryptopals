#ifndef CRYPTOPALS_AES_H
#define CRYPTOPALS_AES_H

#include <stdint.h>

#include <openssl/aes.h>

#define AES_BLOCK_BYTES 16

int aes128_ecb_encrypt(uint8_t *plaintext, size_t len,
                       uint8_t* key, size_t keylen,
                       uint8_t *out);
int aes128_ecb_decrypt(uint8_t *cipher, size_t len,
                       uint8_t* key, size_t keylen,
                       uint8_t *out);

int aes128_cbc_encrypt(uint8_t *plaintext, size_t len,
                       uint8_t* key, size_t keylen,
                       uint8_t* iv, uint8_t *out);
int aes128_cbc_decrypt(uint8_t *cipher, size_t len,
                       uint8_t* key, size_t keylen,
                       uint8_t* iv, uint8_t *out);

#endif /* CRYPTOPALS_AES_H */
