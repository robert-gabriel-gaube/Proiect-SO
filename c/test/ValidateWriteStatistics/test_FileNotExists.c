#include "unity.h"
#include "statistics.h"

void setUp(void) {} 

void tearDown(void) {}

void test_FileNotExists() {
    TEST_ASSERT_FALSE(write_statistics("../a.bmp"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_FileNotExists);
    perror("");
    return UNITY_END();
}