#include "unity.h"
#include "statistics.h"

void setUp(void) {} 

void tearDown(void) {}

void test_FileNoReadPermission() {
    TEST_ASSERT_FALSE(write_statistics("bmp-no-read.bmp"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_FileNoReadPermission);
    perror("");
    return UNITY_END();
}