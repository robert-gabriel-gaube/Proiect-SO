#pragma once 
#include <stdbool.h>
#include <sys/types.h>
#include <stdint.h>

#define WIDTH_POSITION 18
#define HEIGHT_POSITION 22
#define BIT_COUNT_POSITION 28
#define COMPRESSION_POSITION 30
#define RASTER_DATA_POSITION 54

typedef enum return_value { 
    ERROR,
    REGULAR_FILE,
    BMP_FILE,
    DIRECTORY,
    SYMLINK,
    OTHER
}FILETYPE;

typedef struct uint32_optional {
    bool isPresent;
    uint32_t value;
}UINT32_OPTIONAL;

FILETYPE write_statistics(int fd_statistics, const char* file_name);
UINT32_OPTIONAL get_bmp_size_value(int fd_bmp, off_t position);