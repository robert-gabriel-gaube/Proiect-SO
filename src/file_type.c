#include "file_type.h"
#include "generals.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

// Auxiliary

mode_t get_mode(const char* file_path) {
    struct stat statistics;
    if(lstat(file_path, &statistics) == -1) {
        throw_error("");
    }
    return statistics.st_mode;
}

bool is_same_extension(const char *file_name, const char* expected_extension) {
    char *extension = strrchr(file_name, '.');

    if(NULL == extension || strcmp(extension, expected_extension)) {
        return false;
    }
    return true;
}

// Main functions

bool is_regular_file(const char *file_path) {
    mode_t mode = get_mode(file_path);
    return S_ISREG(mode);
}

bool is_bmp_file(const char *file_path) {
    return is_same_extension(file_path, ".bmp");
}

bool is_directory(const char *file_path) {
    mode_t mode = get_mode(file_path);
    return S_ISDIR(mode);
}

bool is_symlink(const char *file_path) {
    mode_t mode = get_mode(file_path);
    return S_ISLNK(mode);
}