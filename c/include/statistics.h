#pragma once 

typedef enum return_value { 
    ERROR,
    REGULAR_FILE,
    BMP_FILE,
    DIRECTORY,
    SYMLINK,
    OTHER
}FILETYPE;

FILETYPE write_statistics(int fd_statistics, const char* file_name);