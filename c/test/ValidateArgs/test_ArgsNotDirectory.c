#include "unity.h"
#include "generals.h"

void setUp(void) {} 

void tearDown(void) {}

void test_NotDirectory() {
    char *ptrs[3] = {"", "Makefile", "out-dir"};
    TEST_ASSERT_FALSE(are_args_valid(2, ptrs));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_NotDirectory);
    perror("");
    return UNITY_END();
}