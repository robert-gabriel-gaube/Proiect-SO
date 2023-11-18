#include "bmp_transform.h"
#include "statistics.h"
#include "generals.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define RED 0
#define GREEN 1
#define BLUE 2

#define RED_WEIGHT 0.299
#define GREEN_WEIGHT 0.587
#define BLUE_WEIGHT 0.114

bool read_from_file(int fd_bmp, void *buffer, off_t position, unsigned nbytes) {
    off_t code = lseek(fd_bmp, position, SEEK_SET);
    if(code == -1) return false;

    if(read(fd_bmp, buffer, nbytes) == -1) return false;
    return true;
}

bool replace_pixels(int fd_bmp, uint8_t grayscale, off_t position) {
    off_t code = lseek(fd_bmp, position, SEEK_SET);
    if(code == -1) return false;

    if(write(fd_bmp, &grayscale, 1) == -1) return false;
    if(write(fd_bmp, &grayscale, 1) == -1) return false;
    if(write(fd_bmp, &grayscale, 1) == -1) return false;

    return true;
}

bool test_compression(int fd_bmp) {
    UINT32_OPTIONAL compression_optional = get_bmp_size_value(fd_bmp, COMPRESSION_POSITION);

    if(compression_optional.isPresent) {
        if(compression_optional.value != 0) {
            fprintf(stderr, "Compression on bmp is not supported at this time");
            return false;
        }
    } 
    else {
        return false;
    }
    return true;
}

bool test_bitcount(int fd_bmp) {
    uint16_t bit_count = 0;
    if(!read_from_file(fd_bmp, &bit_count, BIT_COUNT_POSITION, 2)) return false;
    if(bit_count != 24) {
        fprintf(stderr, "Only RGB format is supported at this time");
        return false;
    }
    return true;
}

bool stop_filter(int fd_bmp) {
    close(fd_bmp);
    return false;
}

bool grayscale_filter(const char *bmp_filepath) {
    PRINT_DEBUG("DEBUG", "Started grayscale filter for %s", bmp_filepath);

    int fd_bmp = open(bmp_filepath, O_RDWR);
    if(fd_bmp == -1) {
        return false;
    }

    if(!test_compression(fd_bmp)) return stop_filter(fd_bmp);
    if(!test_bitcount(fd_bmp)) return stop_filter(fd_bmp);

    uint32_t width, height;

    if(!read_from_file(fd_bmp, &width, WIDTH_POSITION, 4)) return stop_filter(fd_bmp);
    if(!read_from_file(fd_bmp, &height, HEIGHT_POSITION, 4)) return stop_filter(fd_bmp);
    
    uint8_t *RGB_values = (uint8_t*)malloc(width * 3);

    if(RGB_values == NULL) {
        fprintf(stderr, "Not enough space\n");
        return stop_filter(fd_bmp);
    }

    if(lseek(fd_bmp, RASTER_DATA_POSITION, SEEK_SET) == -1) {
        return stop_filter(fd_bmp);
    }

    for(size_t num_lines = 0; num_lines < height; ++num_lines) {
        if(read(fd_bmp, RGB_values, width * 3) == -1) return stop_filter(fd_bmp);
        if(lseek(fd_bmp, -(int32_t)(width * 3), SEEK_CUR) == -1) return stop_filter(fd_bmp);

        for(size_t idx = 0; idx < width * 3; idx += 3) {
            uint8_t grayscale = RGB_values[idx] * RED_WEIGHT +
                                RGB_values[idx + 1] * GREEN_WEIGHT +
                                RGB_values[idx + 2] * BLUE_WEIGHT;

            RGB_values[idx] = grayscale;
            RGB_values[idx + 1] = grayscale;
            RGB_values[idx + 2] = grayscale;
        }
        if(write(fd_bmp, RGB_values, width * 3) == -1) return stop_filter(fd_bmp);
    }

    return true;
}