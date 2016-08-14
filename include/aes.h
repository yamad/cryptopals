#ifndef CRYPTOPALS_AES_H
#define CRYPTOPALS_AES_H

#include <stdint.h>

#include <openssl/aes.h>

int aes128_ecb_decrypt(uint8_t *cipher, size_t len,
                       uint8_t* key, size_t keylen,
                       uint8_t *out);

#endif /* CRYPTOPALS_AES_H */
