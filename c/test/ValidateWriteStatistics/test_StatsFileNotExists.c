#include "unity.h"
#include "statistics.h"
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#define OUTPUT "statistics.txt"

int fd;

void setUp(void) {
    fd = creat(OUTPUT, S_IRUSR | S_IWUSR);
    if(fd == -1) {
        TEST_ABORT();
    }
} 

void tearDown(void) {
    if(close(fd) == -1) {
        TEST_ABORT();
    }
}

void test_StatsFileNotExists() {
    TEST_ASSERT_EQUAL(ERROR, write_statistics(fd, "../a.bmp"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_StatsFileNotExists);
    perror("");
    return UNITY_END();
}