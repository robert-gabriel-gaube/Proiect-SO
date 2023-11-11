#include "unity.h"
#include "statistics.h"

void setUp(void) {} 

void tearDown(void) {}

void test_StatsFileNoReadPermission() {
    TEST_ASSERT_FALSE(write_statistics("bmp-no-read.bmp"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_StatsFileNoReadPermission);
    perror("");
    return UNITY_END();
}