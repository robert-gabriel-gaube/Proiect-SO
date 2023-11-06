#include "generals.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void throw_error(const char* msg) {
    perror(msg);
    exit(-1);
}

bool is_same_extension(const char *file_name, const char* expected_extension) {
    char *extension = strrchr(file_name, '.');

    if(NULL == extension || strcmp(extension, expected_extension)) {
        if(NULL == extension) {
            fprintf(stderr, "File doesn't have extenstion\n");
        }
        else {
            fprintf(stderr, "File with extension '%s' doesn't match expected extenstion '%s'\n", extension, expected_extension);
        }
        return false;
    }

    return true;
}

bool are_args_valid(const int argc, const char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Usage ./program <input_file>\n");
        return false;
    }
    if(!is_same_extension(argv[1], ".bmp")) {
        return false;
    }
    return true;
}