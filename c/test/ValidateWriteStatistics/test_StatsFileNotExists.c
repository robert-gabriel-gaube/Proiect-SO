#include "unity.h"
#include "statistics.h"

void setUp(void) {} 

void tearDown(void) {}

void test_StatsFileNotExists() {
    TEST_ASSERT_FALSE(write_statistics("../a.bmp"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_StatsFileNotExists);
    perror("");
    return UNITY_END();
}