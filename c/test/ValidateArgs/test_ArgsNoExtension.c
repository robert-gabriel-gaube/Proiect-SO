#include "unity.h"
#include "generals.h"

void setUp(void) {} 

void tearDown(void) {}

void test_NoExtension() {
    char *ptrs[2]; char filename[] = "test";
    ptrs[1] = filename;
    TEST_ASSERT_FALSE(are_args_valid(2, ptrs));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_NoExtension);
    return UNITY_END();
}