#include "unity.h"
#include "b64.h"

void test_b64_from(void)
{
	char B64_DIG[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	for (int i = 0; i < 64; i++)
		TEST_ASSERT_EQUAL_INT(i, from_b64(B64_DIG[i]));
}
