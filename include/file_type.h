#pragma once
#include <stdbool.h>

bool is_regular_file(const char *file_path);
bool is_directory(const char *file_path);
bool is_bmp_file(const char *file_path);
bool is_symlink(const char *file_path);