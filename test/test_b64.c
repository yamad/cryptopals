#include <stdlib.h>

#include "unity.h"
#include "b64.h"

static char B64_DIG[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void test_b64_from(void)
{
	for (int i = 0; i < 64; i++)
		TEST_ASSERT_EQUAL_INT(i, from_b64(B64_DIG[i]));

	TEST_ASSERT_EQUAL_INT(-1, from_b64('\n'));
}

void test_b64_decode_group(void)
{
	uint8_t out[4] = {0};
	out[3] = '\0';

	char *i1 = "TWFu";
	b64_decode_group(&i1, out, 3);
	TEST_ASSERT_EQUAL_STRING("Man", out);

	char *i2 = "c3Vy";
	b64_decode_group(&i2, out, 3);
	TEST_ASSERT_EQUAL_STRING("sur", out);

	out[2] = '\0';
	char *i3 = "c3U=";
	b64_decode_group(&i3, out, 3);
	TEST_ASSERT_EQUAL_STRING("su", out);

	char *i4 = "c3U";
	b64_decode_group(&i4, out, 3);
	TEST_ASSERT_EQUAL_STRING("su", out);

	out[1] = '\0';
	char *i5 = "cw==";
	b64_decode_group(&i5, out, 3);
	TEST_ASSERT_EQUAL_STRING("s", out);

	char *i6 = "cw";
	b64_decode_group(&i6, out, 3);
	TEST_ASSERT_EQUAL_STRING("s", out);
}

void test_b64_decode_group_badinput(void)
{
	uint8_t out[4] = {0};
	out[3] = '\0';

	char *in = "T\tWF\nu";
	b64_decode_group(&in, out, 3);
	TEST_ASSERT_EQUAL_STRING("Man", out);
}

void test_b64_next(void)
{
	char *b64 = "\nA";
	TEST_ASSERT_EQUAL_INT('A', b64_next(&b64));
	TEST_ASSERT_EQUAL_INT('\0', *b64);
}

void test_b64_nbytes(void)
{
	TEST_ASSERT_EQUAL_INT(20, b64_nbytes("YW55IGNhcm5hbCBwbGVhc3VyZS4="));
	TEST_ASSERT_EQUAL_INT(19, b64_nbytes("YW55IGNhcm5hbCBwbGVhc3VyZQ=="));
	TEST_ASSERT_EQUAL_INT(18, b64_nbytes("YW55IGNhcm5hbCBwbGVhc3Vy"));
	TEST_ASSERT_EQUAL_INT(17, b64_nbytes("YW55IGNhcm5hbCBwbGVhc3U="));
	TEST_ASSERT_EQUAL_INT(16, b64_nbytes("YW55IGNhcm5hbCBwbGVhcw=="));

	TEST_ASSERT_EQUAL_INT(18, b64_nbytes("YW55IGNhcm5\nhbCBwb^GVhc3Vy"));
}

void test_b64_decode(void)
{
	char *expected;
	uint8_t out[256];
	int n;

	expected = "TWFu";
	n = b64_decode(expected, out, b64_nbytes(expected));
	out[n] = '\0';
	TEST_ASSERT_EQUAL_STRING("Man", out);

	expected = "TQ==";
	n = b64_decode(expected, out, b64_nbytes(expected));
	out[n] = '\0';
	TEST_ASSERT_EQUAL_STRING("M", out);

	expected = "TWE=";
	n = b64_decode(expected, out, b64_nbytes(expected));
	out[n] = '\0';
	TEST_ASSERT_EQUAL_STRING("Ma", out);

	expected = "YW55IGNhcm5hbCBwbGVhc3VyZS4=";
	n = b64_decode(expected, out, b64_nbytes(expected));
	out[n] = '\0';
	TEST_ASSERT_EQUAL_STRING("any carnal pleasure.", out);

	expected = "YW55IGNhcm5hbCBwbGVhc3VyZQ==";
	n = b64_decode(expected, out, b64_nbytes(expected));
	out[n] = '\0';
	TEST_ASSERT_EQUAL_STRING("any carnal pleasure", out);

	expected = "YW55IGNhcm5hbCBwbGVhc3Vy";
	n = b64_decode(expected, out, b64_nbytes(expected));
	out[n] = '\0';
	TEST_ASSERT_EQUAL_STRING("any carnal pleasur", out);

	n = b64_decode(expected, out, 2);
	out[n] = '\0';
	TEST_ASSERT_EQUAL_STRING("an", out);

	n = b64_decode(expected, out, 5);
	out[n] = '\0';
	TEST_ASSERT_EQUAL_STRING("any c", out);

	/* skip non-base64 characters */
	expected = "YW5%5IGNh\ncm5hbC@@BwbGVhc3VyZS4=";
	n = b64_decode(expected, out, b64_nbytes(expected));
	out[n] = '\0';
	TEST_ASSERT_EQUAL_STRING("any carnal pleasure.", out);
}

void test_b64_decode_long(void)
{
	uint8_t expected[135] = {
		0x1d, 0x42, 0x1f, 0x4d, 0x0b, 0x0f, 0x02, 0x1f, 0x4f, 0x13, \
		0x4e, 0x3c, 0x1a, 0x69, 0x65, 0x1f, 0x49, 0x1c, 0x0e, 0x4e, \
		0x13, 0x01, 0x0b, 0x07, 0x4e, 0x1b, 0x01, 0x16, 0x45, 0x36, \
		0x00, 0x1e, 0x01, 0x49, 0x64, 0x20, 0x54, 0x1d, 0x1d, 0x43, \
		0x33, 0x53, 0x4e, 0x65, 0x52, 0x06, 0x00, 0x47, 0x54, 0x1c, \
		0x0d, 0x45, 0x4d, 0x07, 0x04, 0x0c, 0x53, 0x12, 0x3c, 0x0c, \
		0x1e, 0x08, 0x49, 0x1a, 0x09, 0x11, 0x4f, 0x14, 0x4c, 0x21, \
		0x1a, 0x47, 0x2b, 0x00, 0x05, 0x1d, 0x47, 0x59, 0x11, 0x04, \
		0x09, 0x00, 0x64, 0x26, 0x07, 0x53, 0x00, 0x37, 0x16, 0x06, \
		0x0c, 0x1a, 0x17, 0x41, 0x1d, 0x01, 0x52, 0x54, 0x30, 0x5f, \
		0x00, 0x20, 0x13, 0x0a, 0x05, 0x47, 0x4f, 0x12, 0x48, 0x08, \
		0x45, 0x4e, 0x65, 0x3e, 0x16, 0x09, 0x38, 0x45, 0x06, 0x05, \
		0x08, 0x1a, 0x46, 0x07, 0x4f, 0x1f, 0x59, 0x78, 0x7e, 0x6a, \
		0x62, 0x36, 0x0c, 0x1d, 0x0f };

	char *input = "HUIfTQsPAh9PE048GmllH0kcDk4TAQsHThsBFkU2AB4BSWQgVB0dQzNTTmVS\nBgBHVBwNRU0HBAxTEjwMHghJGgkRTxRMIRpHKwAFHUdZEQQJAGQmB1MANxYG\nDBoXQR0BUlQwXwAgEwoFR08SSAhFTmU+Fgk4RQYFCBpGB08fWXh+amI2DB0P";
	uint8_t actual[135] = {0};

	int n = b64_decode(input, actual, 135);
	TEST_ASSERT_EQUAL_INT(135, b64_nbytes(input));
	TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, actual, n);
}

void test_b64_ischar(void)
{
	for (int i = 0; i < 64; i++)
		TEST_ASSERT_MESSAGE(b64_ischar(B64_DIG[i]), "Valid base-64 character not detected");
	TEST_ASSERT_MESSAGE(b64_ischar('='), "B64 pad (=) is not detected");

	for (int i = 0; i < 43; i++)
		TEST_ASSERT_FALSE(b64_ischar((char)i));
}
