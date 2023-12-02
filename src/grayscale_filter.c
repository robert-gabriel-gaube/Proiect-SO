#include "grayscale_filter.h"
#include "statistics.h"
#include "generals.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

#define RED 0
#define GREEN 1
#define BLUE 2

#define RED_WEIGHT 0.299
#define GREEN_WEIGHT 0.587
#define BLUE_WEIGHT 0.114

// Auxiliary

void read_from_file(int fd_bmp, void *buffer, off_t position, unsigned nbytes) {
    if(lseek(fd_bmp, position, SEEK_SET) == ERROR) {
        throw_error("");
    }
    if(read(fd_bmp, buffer, nbytes) == ERROR){
        throw_error("");
    }
}

void test_compression(int fd_bmp) {
    uint32_t compression_value = get_bmp_size_value(fd_bmp, COMPRESSION_POSITION);

    if(compression_value != 0) {
        throw_error("Compression on bmp is not supported at this time");
    }
}

void test_bitcount(int fd_bmp) {
    uint16_t bit_count = 0;
    read_from_file(fd_bmp, &bit_count, BIT_COUNT_POSITION, 2);

    if(bit_count != 24) {
        throw_error("Only RGB format is supported at this time");
    }
}

// Main functions

void apply_grayscale_filter(const char *bmp_file_path) {
    // Open BMP
    int fd_bmp = 0;
    if((fd_bmp = open(bmp_file_path, O_RDWR)) == ERROR) {
        throw_error("");
    }

    // Test corectness
    test_compression(fd_bmp);
    test_bitcount(fd_bmp);

    // Get width, height
    uint32_t width, height;
    read_from_file(fd_bmp, &width, WIDTH_POSITION, 4);
    read_from_file(fd_bmp, &height, HEIGHT_POSITION, 4);
    
    // Initializations
    uint8_t *RGB_values = (uint8_t*)malloc(width * 3);
    if(RGB_values == NULL) {
        throw_error("Not enough space for RGB_values");
    }
    if(lseek(fd_bmp, RASTER_DATA_POSITION, SEEK_SET) == ERROR) {
        free(RGB_values);
        throw_error("");
    }

    // Replace pixels
    for(size_t num_lines = 0; num_lines < height; ++num_lines) {
        // Read 'row' of pixels
        if(read(fd_bmp, RGB_values, width * 3) == ERROR) {
            free(RGB_values);
            throw_error("");
        }

        // Apply grayscale filter
        for(size_t idx = 0; idx < width * 3; idx += 3) {
            uint8_t grayscale = RGB_values[idx] * RED_WEIGHT +
                                RGB_values[idx + 1] * GREEN_WEIGHT +
                                RGB_values[idx + 2] * BLUE_WEIGHT;

            RGB_values[idx] = grayscale;
            RGB_values[idx + 1] = grayscale;
            RGB_values[idx + 2] = grayscale;
        }

        // Reset cursor
        if(lseek(fd_bmp, -(int32_t)(width * 3), SEEK_CUR) == ERROR) {
            free(RGB_values);
            throw_error("");
        }

        // Write new pixels
        if(write(fd_bmp, RGB_values, width * 3) == ERROR) {
            free(RGB_values);
            throw_error("");
        }
    }
    free(RGB_values);
}
