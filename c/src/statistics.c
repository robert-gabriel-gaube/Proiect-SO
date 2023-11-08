#include "statistics.h"
#include "generals.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define OUTPUT "statistics.txt"
#define WIDTH_POSITION 18
#define HEIGHT_POSITION 22
#define SIZE_POSITION 2

#ifdef DEBUG
#define PRINT_DEBUG(TYPE, format, args...) printf("[" TYPE "] " format "\n", ##args)
#else
#define PRINT_DEBUG(TYPE, format, args...)
#endif

typedef struct uint32_optional {
    bool isPresent;
    uint32_t value;
}UINT32_OPTIONAL;

// Generals

bool stop(int fd1, int fd2) {
    close(fd1); close(fd2);
    return false;
}

bool write_to_file(int fd, const char *bytes, size_t numBytes) {
    ssize_t code = write(fd, bytes, numBytes);
    return code == -1 ? false : true;
}

bool write_newline(int fd) {
    return write_to_file(fd, "\n", 1);
}

// File name

bool write_file_name(int fd_output, const char* file_name) {
    if(!write_to_file(fd_output, "file name: ", strlen("file name: "))) return false;
    if(!write_to_file(fd_output, file_name, strlen(file_name))) return false;
    if(!write_newline(fd_output)) return false;
    PRINT_DEBUG("DEBUG", "Wrote %s to %s", file_name, OUTPUT);
    return true;
}

// Dimensions

UINT32_OPTIONAL get_bmp_size_value(int fd_bmp, off_t position) {
    off_t code = lseek(fd_bmp, position, SEEK_SET);

    UINT32_OPTIONAL result; 
    result.isPresent = false; 

    if(code == -1) return result;

    uint32_t size = 0;
    if(read(fd_bmp, &size, 4) == -1) return result;

    result.isPresent = true;
    result.value = size;

    return result;
}

// Write bmp height width and total bytes size to file
bool write_bmp_sizes(int fd_where, int fd_bmp) {
    char labels[3][10] = {"height", "width", "size"};
    UINT32_OPTIONAL values[3] = {
        get_bmp_size_value(fd_bmp, HEIGHT_POSITION),
        get_bmp_size_value(fd_bmp, WIDTH_POSITION),
        get_bmp_size_value(fd_bmp, SIZE_POSITION)
    };

    char string_number[12], message[20];
    for(int idx = 0; idx < 3; ++idx) {
        if(!values[idx].isPresent) return false;

        uint32_t value = values[idx].value;
        PRINT_DEBUG("DEBUG", "Got %s: %u", labels[idx], value);

        sprintf(string_number, "%u", value);
        sprintf(message, "%s: ", labels[idx]);
        if(!write_to_file(fd_where, message, strlen(message))) return false;
        if(!write_to_file(fd_where, string_number, strlen(string_number))) return false;
        if(!write_newline(fd_where)) return false;
        PRINT_DEBUG("DEBUG", "Wrote %s(%u) to %s", labels[idx], value, OUTPUT);
    }
    return true;
}

bool write_statistics(const char* file_name) {
    int fd_statistics = creat(OUTPUT, S_IRUSR | S_IWUSR);

    PRINT_DEBUG("DEBUG", "Opened %s", OUTPUT);
    
    if(fd_statistics == -1) {
        return false;
    }

    int fd_image = open(file_name, O_RDONLY);
    if(fd_image == -1) {
        close(fd_statistics);
        return false;
    }

    PRINT_DEBUG("DEBUG", "Opened %s", file_name);

    if(!write_file_name(fd_statistics, file_name)) return stop(fd_statistics, fd_image);
    if(!write_bmp_sizes(fd_statistics, fd_image)) return stop(fd_statistics, fd_image);

    if(close(fd_statistics) == -1) {
        close(fd_image); 
        return false; 
    }
    PRINT_DEBUG("DEBUG", "Closed %s", OUTPUT);
    if(close(fd_image) == -1) {
        return false;
    }
    PRINT_DEBUG("DEBUG", "Closed %s", file_name);

    return true;
}