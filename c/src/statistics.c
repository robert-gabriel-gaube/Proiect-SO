#include "statistics.h"
#include "generals.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#define OUTPUT "statistics.txt"

void write_statistics(const char* file_name) {
    int file_descriptor = creat(OUTPUT, S_IRUSR | S_IWUSR);
    if(file_descriptor == -1) {
        throw_error("");
    }
    close(file_descriptor);
}