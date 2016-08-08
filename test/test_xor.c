#include <stdint.h>

#include "unity.h"
#include "xor.h"
#include "util.h"

void test_score_keysize(void)
{
	double s = score_keysize((uint8_t *)"this is a testwokka wokka!!!this is a test",
	                         14*3, 14);
	TEST_ASSERT_EQUAL_FLOAT((37.0+0.00+37.00)/(14*3), s);
}
