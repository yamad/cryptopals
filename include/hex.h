#ifndef CRYPTOPALS_HEX_H
#define CRYPTOPALS_HEX_H

/* convert hex string to bytes */
int hex_decode(char *hex, int nbytes, unsigned char *bytes);
/* convert bytes to hex string */
int hex_encode(unsigned char *bytes, int nbytes, char *hex, int nhex);
/* convert hex representation of byte into byte */
short int get_hex_byte(char *in);

#endif /* CRYPTOPALS_HEX_H */
