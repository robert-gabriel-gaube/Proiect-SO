#include "unity.h"
#include "generals.h"

void setUp(void) {} 

void tearDown(void) {}

void test_WrongNumberOfArgsLower() {
    char *ptrs[3];
    TEST_ASSERT_FALSE(are_args_valid(1, ptrs));
}

void test_WrongNumberOfArgsHigher() {
    char *ptrs[3];
    TEST_ASSERT_FALSE(are_args_valid(4, ptrs));
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_WrongNumberOfArgsLower);
    perror("");
    RUN_TEST(test_WrongNumberOfArgsHigher);
    perror("");
    return UNITY_END();
}