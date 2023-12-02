#include "file_type.h"
#include "generals.h"
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Auxiliary

int create_process() {
    int pid = 0;
    if((pid = fork()) == ERROR) {
        throw_error("");
    }
    return pid;
}

void throw_error(const char *msg) {
    perror(msg);
    exit(ERROR);
}

// Main functions

bool are_args_valid(int argc, char** argv) {
    // Test args number
    if(argc != 4) {
        fprintf(stderr, "Usage ./program <input_directory> <output_directory> <c>\n");
        return false;
    }

    // Test args correctness
    if(!is_directory(argv[1])) {
        fprintf(stderr, "Given '%s' is not a valid directory\n", argv[1]);
        return false;
    }
    if(!is_directory(argv[2])) {
        fprintf(stderr, "Given '%s' is not a valid directory\n", argv[2]);
        return false;
    }
    if(strlen(argv[3]) != 1) {
        fprintf(stderr, "'%s' should be only one character\n", argv[3]);
        return false;
    }
    
    // All good
    return true;
}