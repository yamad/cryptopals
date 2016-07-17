#include "util.h"
#include "hex_to_b64.c"

#define MAXBYTE 1024

int main(int argc, char *argv[])
{
    unsigned char bytes[MAXBYTE] = {0};
    char b64_buf[MAXBYTE] = {0};

    ensure_argc(2, "Usage: %s <hex value>\n");

    int nbytes = decode_hex(argv[1], MAXBYTE, bytes); /* hex to bytes */
    encode_b64(bytes, nbytes, b64_buf);               /* bytes to b64 */
    printf("%s\n", b64_buf);                          /* output */
    return 0;
}
