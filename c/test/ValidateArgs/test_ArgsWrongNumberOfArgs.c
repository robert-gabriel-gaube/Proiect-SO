#include "unity.h"
#include "generals.h"

void setUp(void) {} 

void tearDown(void) {}

void test_WrongNumberOfArgsLower() {
    char *ptrs[2];
    TEST_ASSERT_FALSE(are_args_valid(1, ptrs));
}

void test_WrongNumberOfArgsHigher() {
    char *ptrs[2];
    TEST_ASSERT_FALSE(are_args_valid(3, ptrs));
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_WrongNumberOfArgsLower);
    RUN_TEST(test_WrongNumberOfArgsHigher);
    return UNITY_END();
}