#include "generals.h"
#include "statistics.h"
#include <stdio.h>

int main(int argc, char **argv) {
    if(!are_args_valid(argc, argv)) {
        throw_error("");
    }
    write_statistics(argv[1]);
    return 0;
}