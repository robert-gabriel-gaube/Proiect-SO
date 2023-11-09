#include "unity.h"
#include "statistics.h"
#include <errno.h>

void setUp(void) {} 

void tearDown(void) {}

void test_GoodBmp() {
    TEST_ASSERT_TRUE(write_statistics("./resources-test/good-bmp.bmp"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_GoodBmp);
    perror("");
    return UNITY_END();
}