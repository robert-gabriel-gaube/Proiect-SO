#include "statistics.h"
#include "generals.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

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

bool write_descriptor_value_to_file(int fd, const char* descriptor, const char* value) {
    if(!write_to_file(fd, descriptor, strlen(descriptor))) return false;
    if(!write_to_file(fd, value, strlen(value))) return false;
    if(!write_newline(fd)) return false;
    return true;
}

// File name

bool write_file_name(int fd_output, const char* file_name) {
    if(!write_descriptor_value_to_file(fd_output, "file name: \0", file_name)) return false;
    
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
        if(!write_descriptor_value_to_file(fd_where, message, string_number)) return false;
        PRINT_DEBUG("DEBUG", "Wrote %s(%u) to %s", labels[idx], value, OUTPUT);
    }
    return true;
}

// Stat information

bool write_modified_date(int fd_statistics, struct timespec time) {
    struct tm localtime_struct;
    struct tm *retptr = localtime_r(&(time.tv_sec), &localtime_struct);

    if(retptr == NULL) return false;

    char time_str[30];
    size_t bytes_written = strftime(time_str, sizeof(time_str), "%d.%m.%Y", &localtime_struct);
    if(bytes_written == 0) return false;
    
    if(!write_descriptor_value_to_file(fd_statistics, "last modified date: \0", time_str)) return false;
    PRINT_DEBUG("DEBUG", "Wrote last modified date(%s) to %s", time_str, OUTPUT);
    
    return true;
}

bool write_permissions(int fd_statistics, mode_t mode) {
    char user_permissions[4], group_permissions[4], other_permissions[4];
    user_permissions[3] = group_permissions[3] = other_permissions[3] = 0;
    int user_permissions_values[3] = {S_IRUSR, S_IWUSR, S_IXUSR};
    int group_permissions_values[3] = {S_IRGRP, S_IWGRP, S_IXGRP};
    int other_permissions_values[3] = {S_IROTH, S_IWOTH, S_IXOTH};
    char encodings[] = "RWX";

    for(int idx = 0; idx < 3; ++idx) {
        user_permissions[idx] = (user_permissions_values[idx] & mode) ? encodings[idx] : '-';
        group_permissions[idx] = (group_permissions_values[idx] & mode) ? encodings[idx] : '-';
        other_permissions[idx] = (other_permissions_values[idx] & mode) ? encodings[idx] : '-';
    }   
    
    if(!write_descriptor_value_to_file(fd_statistics, "user permissions: \0", user_permissions)) return false;
    PRINT_DEBUG("DEBUG", "Wrote user permissions(%s) to %s", user_permissions, OUTPUT);
    if(!write_descriptor_value_to_file(fd_statistics, "group permissions: \0", group_permissions)) return false;
    PRINT_DEBUG("DEBUG", "Wrote group permissions(%s) to %s", group_permissions, OUTPUT);
    if(!write_descriptor_value_to_file(fd_statistics, "others permission: \0", other_permissions)) return false;
    PRINT_DEBUG("DEBUG", "Wrote others permissions(%s) to %s", other_permissions, OUTPUT);

    return true;
}

bool write_stat_information(int fd_statistics, const char* file_name) {
    struct stat statistics;
    if(stat(file_name, &statistics) == -1) {
        return false;
    }

    PRINT_DEBUG("DEBUG", "Got '%s' statistics", file_name);

    char string_number[20];
    sprintf(string_number, "%u", statistics.st_uid);
    if(!write_descriptor_value_to_file(fd_statistics, "user identificator: \0", string_number)) return false;
    PRINT_DEBUG("DEBUG", "Wrote user id(%s) to %s", string_number, OUTPUT);

    if(!write_modified_date(fd_statistics, statistics.st_mtim)) return false;

    sprintf(string_number, "%lu", statistics.st_nlink);
    if(!write_descriptor_value_to_file(fd_statistics, "number of hardlinks: \0", string_number)) return false;
    PRINT_DEBUG("DEBUG", "Wrote hardlink number(%s) to %s", string_number, OUTPUT);

    if(!write_permissions(fd_statistics, statistics.st_mode)) return false;

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
    if(!write_stat_information(fd_statistics, file_name)) return stop(fd_statistics, fd_image);

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