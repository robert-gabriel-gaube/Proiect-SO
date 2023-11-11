#include "unity.h"
#include "statistics.h"
#include <errno.h>
#include <string.h>

void setUp(void) {} 

void tearDown(void) {}

bool are_files_same(FILE *file1, FILE *file2) {
    const size_t buffer_size = 1024;
    char buffer1[buffer_size];
    char buffer2[buffer_size];
    size_t bytesRead1, bytesRead2;

    do {
        bytesRead1 = fread(buffer1, 1, buffer_size, file1);
        bytesRead2 = fread(buffer2, 1, buffer_size, file2);

        buffer1[bytesRead1] = 0;
        buffer2[bytesRead2] = 0;
        
        if (bytesRead1 != bytesRead2 || strcmp(buffer1, buffer2) != 0) {
            return false;
        }
    } while (bytesRead1 > 0);

    return true;
}

void test_StatsGoodRegularFile() {
    TEST_ASSERT_TRUE(write_statistics("./resources-test/sample.txt"));   
    FILE *statistics = NULL, *golden_data = NULL;
    if((statistics = fopen("statistics.txt", "rb")) == NULL) {
        TEST_FAIL_MESSAGE("Couldn't open statistics.txt");
    }
    if((golden_data = fopen("./resources-test/good-regular-file-statistics.txt", "rb")) == NULL) {
        fclose(statistics);
        TEST_FAIL_MESSAGE("Couldn't open golden data");
    }
    TEST_ASSERT_TRUE(are_files_same(statistics, golden_data));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_StatsGoodRegularFile);
    perror("");
    return UNITY_END();
}