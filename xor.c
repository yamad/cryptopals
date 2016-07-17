int fixed_xor(unsigned char* buf1, unsigned char *buf2, int len, unsigned char* dst);

int fixed_xor(unsigned char* buf1, unsigned char *buf2, int len, unsigned char* dst) {
	int n;
	for (n=0; n < len; n++) {
		dst[n] = (unsigned char)(buf1[n] ^ buf2[n]);
	}
	return n;
}
