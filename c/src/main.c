#include "generals.h"
#include "statistics.h"
#include <stdio.h>

int main(int argc, char **argv) {
    if(!are_args_valid(argc, argv)) {
        perror("");
        return -1;
    }
    if(!write_statistics(argv[1])) {
        perror("");
        return -1;
    };
    return 0;
}