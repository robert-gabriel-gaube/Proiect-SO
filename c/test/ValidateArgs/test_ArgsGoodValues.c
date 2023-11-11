#include "unity.h"
#include "generals.h"

void setUp(void) {} 

void tearDown(void) {}

void test_GoodValues() {
    char *ptrs[2]; char filename[] = "test-dir";
    ptrs[1] = filename;
    TEST_ASSERT_TRUE(are_args_valid(2, ptrs));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_GoodValues);
    perror("");
    return UNITY_END();
}