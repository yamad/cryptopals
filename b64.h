#ifndef CRYPTOPALS_B64_H
#define CRYPTOPALS_B64_H

unsigned char get_n_6bit(unsigned char *bytes, unsigned int n);
int encode_b64(unsigned char *bytes, int nbytes, char *b64);

#endif /* CRYPTOPALS_B64_H */
