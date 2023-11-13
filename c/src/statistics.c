#include "statistics.h"
#include "generals.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
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

bool stop(int fd1) {
    close(fd1);
    return ERROR;
}

bool write_to_file(int fd, const char *bytes) {
    ssize_t code = write(fd, bytes, strlen(bytes));
    return code == -1 ? false : true;
}

bool write_newline(int fd) {
    return write_to_file(fd, "\n");
}

bool write_descriptor_value_to_file(int fd, const char* descriptor, const char* value) {
    if(!write_to_file(fd, descriptor)) return false;
    if(!write_to_file(fd, value)) return false;
    if(!write_newline(fd)) return false;
    return true;
}

// File name

bool write_file_name(int fd_output, const char* file_name) {
    char *actual_file_name = strrchr(file_name, '/');
    if(actual_file_name == NULL) {
        if(!write_to_file(fd_output, file_name)) return false;
        PRINT_DEBUG("DEBUG", "Wrote '%s' to %s", file_name, OUTPUT);
    } 
    else {
        ++actual_file_name; // Jump over '/'
        if(!write_to_file(fd_output, actual_file_name)) return false;
        PRINT_DEBUG("DEBUG", "Wrote '%s' to %s", actual_file_name, OUTPUT);
    }
    if(!write_newline(fd_output)) return false;
    
    
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

bool write_size(int fd_out, const char* msg, off_t size) {
    char string_number[20]; sprintf(string_number, "%lu", size);
    return write_descriptor_value_to_file(fd_out, msg, string_number);
}

// Write bmp height width and total bytes size to file
bool write_bmp_sizes(int fd_where, int fd_bmp) {
    char labels[2][10] = {"height", "width"};
    UINT32_OPTIONAL values[2] = {
        get_bmp_size_value(fd_bmp, HEIGHT_POSITION),
        get_bmp_size_value(fd_bmp, WIDTH_POSITION),
    };

    char string_number[12], message[20];
    for(int idx = 0; idx < 2; ++idx) {
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
    
    char user_text[50];
    char group_text[50];
    char others_text[50];

    if(S_ISLNK(mode)) {
        strcpy(user_text, "user permissions link: ");
        strcpy(group_text, "group permissions link: ");
        strcpy(others_text, "others permissions link: ");
    } 
    else {
        strcpy(user_text, "user permissions: ");
        strcpy(group_text, "group permissions: ");
        strcpy(others_text, "others permissions: ");
    }

    if(!write_descriptor_value_to_file(fd_statistics, user_text, user_permissions)) return false;
    PRINT_DEBUG("DEBUG", "Wrote user permissions(%s) to %s", user_permissions, OUTPUT);
    if(!write_descriptor_value_to_file(fd_statistics, group_text, group_permissions)) return false;
    PRINT_DEBUG("DEBUG", "Wrote group permissions(%s) to %s", group_permissions, OUTPUT);
    if(!write_descriptor_value_to_file(fd_statistics, others_text, other_permissions)) return false;
    PRINT_DEBUG("DEBUG", "Wrote others permissions(%s) to %s", other_permissions, OUTPUT);

    return true;
}

bool write_user_id(int fd_statistics, uid_t uid) {
    char string_number[20];
    sprintf(string_number, "%u", uid);
    if(!write_descriptor_value_to_file(fd_statistics, "user identificator: \0", string_number)) return false;
    PRINT_DEBUG("DEBUG", "Wrote user id(%s) to %s", string_number, OUTPUT);
    return true;
}
bool write_number_hardlinks(int fd_statistics, nlink_t links) {
    char string_number[20];
    sprintf(string_number, "%lu", links);
    if(!write_descriptor_value_to_file(fd_statistics, "number of hardlinks: \0", string_number)) return false;
    PRINT_DEBUG("DEBUG", "Wrote hardlink number(%s) to %s", string_number, OUTPUT);
    return true;
}

FILETYPE write_statistics(int fd_statistics, const char* file_name) {
    PRINT_DEBUG("DEBUG", "Started writing statistics");

    FILETYPE filetype = ERROR;

    int fd_file = open(file_name, O_RDONLY);
    if(fd_file == -1) {
        return ERROR;
    }

    PRINT_DEBUG("DEBUG", "Opened %s", file_name);

    struct stat statistics;
    if(lstat(file_name, &statistics) == -1) {
        return stop(fd_file);
    }

    PRINT_DEBUG("DEBUG", "Got '%s' statistics", file_name);

    if(S_ISLNK(statistics.st_mode)) {
        filetype = SYMLINK;

        PRINT_DEBUG("DEBUG", "'%s' is symlink", file_name);
        mode_t link_mode = statistics.st_mode;

        if(!write_to_file(fd_statistics, "symlink name: ")) return stop(fd_file);
        if(!write_file_name(fd_statistics, file_name)) return stop(fd_file);
        if(!write_size(fd_statistics, "link size: ", statistics.st_size)) return stop(fd_file);

        if(stat(file_name, &statistics) == -1) {
            return stop(fd_file);
        }

        if(!write_size(fd_statistics, "file size: ", statistics.st_size)) return stop(fd_file);
        if(!write_permissions(fd_statistics, link_mode)) return stop(fd_file);
    }
    else if(S_ISREG(statistics.st_mode)) {
        if(!write_to_file(fd_statistics, "file name: ")) return stop(fd_file);
        if(!write_file_name(fd_statistics, file_name)) return stop(fd_file);

        if(is_same_extension(file_name, ".bmp")) {
            PRINT_DEBUG("DEBUG", "'%s' is bmp file", file_name);
            if(!write_bmp_sizes(fd_statistics, fd_file)) return stop(fd_file);
            filetype = BMP_FILE;
        } else {
            PRINT_DEBUG("DEBUG", "'%s' is regular file", file_name);
            filetype = REGULAR_FILE;
        }
        
        if(!write_size(fd_statistics, "size: ", statistics.st_size)) return stop(fd_file);
        if(!write_user_id(fd_statistics, statistics.st_uid)) return stop(fd_file);
        if(!write_modified_date(fd_statistics, statistics.st_mtim)) return stop(fd_file);
        if(!write_number_hardlinks(fd_statistics, statistics.st_nlink)) return stop(fd_file);
        if(!write_permissions(fd_statistics, statistics.st_mode)) return stop(fd_file);
    } 
    else if(S_ISDIR(statistics.st_mode)) {
        filetype = DIRECTORY;

        PRINT_DEBUG("DEBUG", "'%s' is directory", file_name);
        if(!write_to_file(fd_statistics, "directory name: ")) return stop(fd_file);
        if(!write_file_name(fd_statistics, file_name)) return stop(fd_file);
        if(!write_user_id(fd_statistics, statistics.st_uid)) return stop(fd_file);
        if(!write_permissions(fd_statistics, statistics.st_mode)) return stop(fd_file);
    }
    else {
        filetype = OTHER;
    }
    if(close(fd_file) == -1) {
        return ERROR;
    }

    PRINT_DEBUG("DEBUG", "Closed %s", file_name);

    if(!write_newline(fd_statistics)) return ERROR;

    PRINT_DEBUG("DEBUG", "Completed writing statistics\n");
    return filetype;
}