#include "generals.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>

bool is_same_extension(char *file_name, char* expected_extension) {
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

bool are_args_valid(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Usage ./program <input_file>\n");
        return false;
    }
    if(!is_same_extension(argv[1], ".bmp")) {
        return false;
    }
    return true;
}