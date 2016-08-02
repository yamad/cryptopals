#include <stdlib.h>

#include "unity.h"
#include "b64.h"

void test_b64_from(void)
{
	char B64_DIG[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	for (int i = 0; i < 64; i++)
		TEST_ASSERT_EQUAL_INT(i, from_b64(B64_DIG[i]));
}

void test_b64_decode_group(void)
{
	uint8_t out[4] = {0};
	out[3] = '\0';

	b64_decode_group("TWFu", out, 3);
	TEST_ASSERT_EQUAL_STRING("Man", out);

	b64_decode_group("c3Vy", out, 3);
	TEST_ASSERT_EQUAL_STRING("sur", out);

	out[2] = '\0';
	b64_decode_group("c3U=", out, 3);
	TEST_ASSERT_EQUAL_STRING("su", out);

	b64_decode_group("c3U", out, 3);
	TEST_ASSERT_EQUAL_STRING("su", out);

	out[1] = '\0';
	b64_decode_group("cw==", out, 3);
	TEST_ASSERT_EQUAL_STRING("s", out);

	b64_decode_group("cw", out, 3);
	TEST_ASSERT_EQUAL_STRING("s", out);
}

void test_b64_nbytes(void)
{
	TEST_ASSERT_EQUAL_INT(20, b64_nbytes("YW55IGNhcm5hbCBwbGVhc3VyZS4="));
	TEST_ASSERT_EQUAL_INT(19, b64_nbytes("YW55IGNhcm5hbCBwbGVhc3VyZQ=="));
	TEST_ASSERT_EQUAL_INT(18, b64_nbytes("YW55IGNhcm5hbCBwbGVhc3Vy"));
	TEST_ASSERT_EQUAL_INT(17, b64_nbytes("YW55IGNhcm5hbCBwbGVhc3U="));
	TEST_ASSERT_EQUAL_INT(16, b64_nbytes("YW55IGNhcm5hbCBwbGVhcw=="));
}

void test_b64_decode(void)
{
	char *expected;
	uint8_t out[21];
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
}
