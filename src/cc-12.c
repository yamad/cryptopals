#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "aes.h"
#include "b64.h"
#include "util.h"
#include "buffer.h"

#define MAXLEN 16384

int is_stable_key_set = 0;
char *b64_secret = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

struct probe {
    buffer_dt *buf;
    size_t blocksize;
    size_t index;
    size_t offset;
    size_t iblock;
    size_t blockbyte;
    size_t length;
};
typedef struct probe probe_dt;

bool probe_alloc(probe_dt **probe, size_t blocksize)
{
    *probe = malloc(sizeof(probe_dt));
    if (*probe == NULL)
        return false;

    (*probe)->blocksize = blocksize;
    (*probe)->index = 0;

    return buffer_dt_alloc(&((*probe)->buf), MAXLEN);
}

bool probe_construct(struct probe *probe, buffer_dt *discovered, size_t ibyte)
{
    probe->index     = ibyte;
    probe->iblock    = probe->index / probe->blocksize;
    probe->blockbyte = probe->index % probe->blocksize;
    probe->offset    = probe->blocksize - probe->blockbyte;
    probe->length    = probe->blocksize * (probe->iblock + 1);

    if (!buffer_dt_memset(probe->buf, 'A',
                          probe->offset - 1, 0))
        return false;
    if (!buffer_dt_memcpy(probe->buf,
                          discovered->data,
                          discovered->length,
                          probe->offset - 1))
        return false;
    return true;
}

void probe_set_last_byte(probe_dt *probe, uint8_t b)
{
    buffer_dt_set(probe->buf, probe->length - 1, b);
}

int oracle(uint8_t *plaintext, size_t len, uint8_t *out)
{
    /* obtain single random key on first call */
    static uint8_t stable_key[AES_BLOCK_BYTES];
    if (!is_stable_key_set) {
        aes128_random_key(stable_key);
        is_stable_key_set = 1;
    }

    uint8_t secret[MAXLEN];
    int b64_len = b64_nbytes(b64_secret);
    int secret_len = b64_decode(b64_secret, secret, b64_len);

    int inlen = len + secret_len;
    uint8_t *input = malloc(inlen * sizeof(uint8_t));
    if (input == NULL)
        return -1;
    memcpy(input, plaintext, len);
    memcpy(input+len, secret, secret_len);

    int outlen = aes128_ecb_encrypt(input, inlen, stable_key,
                                    AES_BLOCK_BYTES, out);

    free(input);
    return outlen;
}

void probe_all_responses(probe_dt *probe, uint8_t **responses)
{
    uint8_t probe_response[MAXLEN] = {0};
    size_t block_offset = probe->iblock * probe->blocksize;
    uint8_t *response_block = probe_response + block_offset;

    for (int i = 0; i < 256; i++) {
        probe_set_last_byte(probe, i);
        oracle(probe->buf->data, probe->length, probe_response);
        memcpy(responses[i],
               probe_response + (probe->iblock * probe->blocksize),
               probe->blocksize);
    }
}

void probe_free(struct probe *probe) {
    buffer_dt_free(probe->buf);
}

/** Detect block size of the block encryption function `fencrypt`
 *
 * Finds a jump in the output size of the ciphertext.
 */
int get_blocksize(int (*fencrypt)(uint8_t *, size_t, uint8_t *), uint8_t *out)
{
    uint8_t buf[128];
    memset(buf, 'A', 128);

    int cipherlen;
    int prev = fencrypt(buf, 1, out);
    for (int i = 2; i < 128; i++) {
        cipherlen = fencrypt(buf, i, out);
        if (cipherlen > prev)
            return (cipherlen - prev);
        prev = cipherlen;
    }
    return -1;
}

/** Detect if block encryption function `fencrypt` uses ECB mode
 *
 * Feeds in a block with repeated text and looks for identical
 * blocks. Identical blocks of plain text will encrypt to identical
 * blocks of cipher text in ECB mode.
 */
bool is_ecb(int (*fencrypt)(uint8_t *, size_t, uint8_t *))
{
    uint8_t buf[MAXLEN];
    memset(buf, 'A', MAXLEN);
    uint8_t out[MAXLEN] = {0};
    int outlen = fencrypt(buf, MAXLEN, out);
    return detect_ecb(out, outlen);
}


int main(int argc, char *argv[])
{
    uint8_t cipher[MAXLEN] = {0};

    size_t blksize = get_blocksize(&oracle, cipher);
    if (blksize == -1) {
        printf("Blocksize not detected\n");
        return -1;
    }

    bool ecb = is_ecb(&oracle);
    if (!ecb) {
        printf("Not in ECB mode. Can't decrypt\n");
        return -1;
    }

    bool result = true;

    buffer_dt* discovered = NULL;
    result = buffer_dt_alloc(&discovered, MAXLEN);
    if (!result) return -1;

    uint8_t *responses[256];
    for (int i = 0; i < 256; i++)
        responses[i] = malloc(blksize * sizeof(uint8_t));

    struct probe *probe;
    probe_alloc(&probe, blksize);

    uint8_t probe_response[MAXLEN] = {0};

    int ibyte = 0;
    bool found = true;
    while (found) {
        size_t iblock = ibyte / blksize;

        probe_construct(probe, discovered, ibyte);
        probe_all_responses(probe, responses);
        oracle(probe->buf->data, probe->offset, probe_response);

        found = false;
        for (int i = 0; i < 256; i++) {
            if (hamming_distance(probe_response+(probe->iblock * probe->blocksize),
                                 responses[i], probe->blocksize) == 0) {
                buffer_dt_push(discovered, i);
                found = true;
                break;
            }
        }
        ++ibyte;
    }

    probe_free(probe);

    for (int i = 0; i < 256; i++)
        free(responses[i]);

    uint8_t *b;
    buffer_dt_pop(discovered, b);
    buffer_dt_print_string(discovered);
    buffer_dt_free(discovered);
    return 0;
}
