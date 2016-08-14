#include "unity.h"
#include "util.h"

void test_count_bits(void)
{
	TEST_ASSERT_EQUAL_INT(0, count_bits(0x00));
	TEST_ASSERT_EQUAL_INT(1, count_bits(0x01));
	TEST_ASSERT_EQUAL_INT(1, count_bits(0x02));
	TEST_ASSERT_EQUAL_INT(2, count_bits(0x03));
	TEST_ASSERT_EQUAL_INT(1, count_bits(0x04));
	TEST_ASSERT_EQUAL_INT(2, count_bits(0x05));
	TEST_ASSERT_EQUAL_INT(2, count_bits(0x06));
	TEST_ASSERT_EQUAL_INT(3, count_bits(0x07));
	TEST_ASSERT_EQUAL_INT(1, count_bits(0x08));
	TEST_ASSERT_EQUAL_INT(1, count_bits(0x10));
}

void test_hamming_distance(void)
{
	TEST_ASSERT_EQUAL_INT(37, hamming_distance((uint8_t *)"this is a test",
	                                           (uint8_t *)"wokka wokka!!!", 14));

	/* can compare byte arrays with 0x00 members */
	TEST_ASSERT_EQUAL_INT(2,  hamming_distance((uint8_t[]){0x00, 0x10, 0x00},
	                                           (uint8_t[]){0x10, 0x00, 0x00}, 3));
}

void test_transpose_blocks(void)
{
	uint8_t buf[20] = {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, \
	                    10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

	uint8_t expected[5][4] = { { 0, 5, 10, 15 }, \
	                           { 1, 6, 11, 16 }, \
	                           { 2, 7, 12, 17 }, \
	                           { 3, 8, 13, 18 }, \
	                           { 4, 9, 14, 19 } };

	uint8_t expected2[4][5] = { { 0, 4,  8, 12, 16}, \
	                            { 1, 5,  9, 13, 17}, \
	                            { 2, 6, 10, 14, 18}, \
	                            { 3, 7, 11, 15, 19} };
	uint8_t **t;
	transpose_blocks(buf, 20, 5, &t);
	for (int i = 0; i < 5; i++)
		TEST_ASSERT_EQUAL_UINT8_ARRAY(expected[i], t[i], 4);

	transpose_blocks(buf, 20, 4, &t);
	for (int i = 0; i < 4; i++)
		TEST_ASSERT_EQUAL_UINT8_ARRAY(expected2[i], t[i], 5);
}

void test_fill_block(void)
{
	uint8_t input[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	uint8_t actual[10] = {0};
	int n;

	n = fill_block(input, 10, 2, 0, actual);
	uint8_t expect[5] = { 0, 2, 4, 6, 8 };
	TEST_ASSERT_EQUAL_UINT8_ARRAY(expect, actual, n);

	n = fill_block(input, 10, 2, 1, actual);
	uint8_t expect2[5] = { 1, 3, 5, 7, 9 };
	TEST_ASSERT_EQUAL_UINT8_ARRAY(expect2, actual, n);

	n = fill_block(input, 10, 6, 2, actual);
	uint8_t expect3[2] = { 2, 8 };
	TEST_ASSERT_EQUAL_UINT8_ARRAY(expect3, actual, n);
}

void test_pad_pkcs7(void)
{

	uint8_t input[4]     =   { 'A', 'B', 'C', 'D' };

	/* length of output buffer must be 2x input */
	uint8_t expect[5][8] = { {   4,   4,   4,   4, 0, 0, 0, 0 },
	                         { 'A',   3,   3,   3, 0, 0, 0, 0 },
	                         { 'A', 'B',   2,   2, 0, 0, 0, 0 },
	                         { 'A', 'B', 'C',   1, 0, 0, 0, 0 },
	                         { 'A', 'B', 'C', 'D', 4, 4, 4, 4 } };

	uint8_t actual[8] = {0};
	for (int i = 0; i < 5; i++) {
		pad_pkcs7(input, i, actual, 4);
		TEST_ASSERT_EQUAL_UINT8_ARRAY(expect[i], actual, 8);
	}
}
