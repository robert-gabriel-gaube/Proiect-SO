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

void test_StatsFileNoReadPermission() {
    TEST_ASSERT_EQUAL(ERROR, write_statistics(fd, "bmp-no-read.bmp"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_StatsFileNoReadPermission);
    perror("");
    return UNITY_END();
}