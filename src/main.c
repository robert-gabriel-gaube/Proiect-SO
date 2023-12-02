#include "process_directory.h"
#include "generals.h"
#include <stdio.h>

int main(int argc, char **argv) {
    if(!are_args_valid(argc, argv)) {
        perror("");
        return -1;
    }
    process_directory(argv[1], argv[2], argv[3][0]);
    return 0;
}