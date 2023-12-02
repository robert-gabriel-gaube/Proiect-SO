#pragma once 
#include "stdbool.h"

void process_directory_file(const char *file_path, const char *statistics_file_path);
void process_regular_file(const char *file_path, const char *statistics_file_path, char c);
void process_symlink_file(const char *file_path, const char *statistics_file_path);
void process_bmp_file(const char *file_path, const char *statistics_file_path);