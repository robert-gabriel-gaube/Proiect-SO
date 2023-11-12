#include "generals.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

bool is_same_extension(const char *file_name, const char* expected_extension) {
    char *extension = strrchr(file_name, '.');

    if(NULL == extension || strcmp(extension, expected_extension)) {
        return false;
    }
    return true;
}

bool are_args_valid(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Usage ./program <input_directory>\n");
        return false;
    }
    struct stat statistics;
    if(stat(argv[1], &statistics) == -1){
        return false;
    }
    if(!S_ISDIR(statistics.st_mode)) {
        fprintf(stderr, "File is not directory\n");
        return false;
    }

    return true;
}