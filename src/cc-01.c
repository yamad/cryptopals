#include "util.h"
#include "hex.h"
#include "b64.h"

#define MAXBYTE 1024

int main(int argc, char *argv[])
{
    unsigned char bytes[MAXBYTE] = {0};
    char b64_buf[MAXBYTE] = {0};

    ensure_argc(2, "Usage: %s <hex value>\n");

    int nbytes = hex_decode(argv[1], MAXBYTE, bytes); /* hex to bytes */
    b64_encode(bytes, nbytes, b64_buf);               /* bytes to b64 */
    printf("%s\n", b64_buf);                          /* output */
    return 0;
}
