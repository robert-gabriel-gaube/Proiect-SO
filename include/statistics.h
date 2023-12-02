#pragma once 
#include <sys/types.h>
#include <stdint.h>

#define WIDTH_POSITION 18
#define HEIGHT_POSITION 22
#define BIT_COUNT_POSITION 28
#define COMPRESSION_POSITION 30
#define RASTER_DATA_POSITION 54

uint32_t get_bmp_size_value(int fd_bmp, off_t position);

void write_statistics_directory_file(const char *file_path, const char *statistics_file_path);
void write_statistics_regular_file(const char *file_path, const char *statistics_file_path);
void write_statistics_bmp_file(const char *file_path, const char *statistics_file_path);
void write_statistics_symlink_file(const char *file_path, const char *statistics_file_path);