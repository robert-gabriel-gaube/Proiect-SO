#include "statistics.h"
#include "generals.h"
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

// Auxiliary

void write_to_file(int fd, const char *bytes) {
    ssize_t code = write(fd, bytes, strlen(bytes));
    if(code == -1) {
        throw_error("");
    }
}
void write_newline(int fd) {
    write_to_file(fd, "\n");
}
void write_descriptor_value_to_file(int fd, const char* descriptor, const char* value) {
    write_to_file(fd, descriptor);
    write_to_file(fd, value);
    write_newline(fd);
}
struct stat get_statistics(const char *file_path) {
    struct stat statistics;
    if(lstat(file_path, &statistics) == ERROR) {
        throw_error("");
    }
    return statistics;
}
int get_statistics_file_descriptor(const char *output_file_path) {
    int fd_statistics = creat(output_file_path, S_IRUSR | S_IWUSR);
    if(fd_statistics == ERROR) {
        throw_error("");
    }
    return fd_statistics;
}
uint32_t get_bmp_size_value(int fd_bmp, off_t position) {
    if(lseek(fd_bmp, position, SEEK_SET) == ERROR) {
        throw_error("");
    }

    uint32_t size = 0;
    if(read(fd_bmp, &size, 4) == ERROR) {
        throw_error("");
    }
    return size;
}

// ###########################

void write_file_name(int fd_output, const char* label, const char* file_path) {
    write_to_file(fd_output, label);

    char *actual_file_path = strrchr(file_path, '/');
    if(actual_file_path == NULL) {
        write_to_file(fd_output, file_path);
    } 
    else {
        ++actual_file_path; // Jump over '/'
        write_to_file(fd_output, actual_file_path);
    }
    write_newline(fd_output);
}

void write_size(int fd_out, const char* msg, off_t size) {
    char string_number[20]; sprintf(string_number, "%lu", size);
    write_descriptor_value_to_file(fd_out, msg, string_number);
}

void write_user_id(int fd_statistics, uid_t uid) {
    char string_number[20]; sprintf(string_number, "%u", uid);
    write_descriptor_value_to_file(fd_statistics, "user identificator: ", string_number);
}

void write_modified_date(int fd_statistics, struct timespec time) {
    struct tm localtime_struct;

    // Transform Epoch time to tm struct (_r for putting in a struct as opposed to returning it)
    if(localtime_r(&(time.tv_sec), &localtime_struct) == NULL) {
        throw_error("Time couldn't be transformed");
    }

    // Convert it to human readable
    char time_str[30];
    if(strftime(time_str, sizeof(time_str), "%d.%m.%Y", &localtime_struct) == 0) {
        throw_error("Time couldn't be consumed");
    }
    
    write_descriptor_value_to_file(fd_statistics, "last modified date: ", time_str);
}

void write_number_hardlinks(int fd_statistics, nlink_t links) {
    char string_number[20]; sprintf(string_number, "%lu", links);
    write_descriptor_value_to_file(fd_statistics, "number of hardlinks: ", string_number);
}

void write_permissions(int fd_statistics, mode_t mode) {
    // Initializations
    char user_permissions[4], group_permissions[4], other_permissions[4];
    user_permissions[3] = group_permissions[3] = other_permissions[3] = 0;
    int user_permissions_values[3] = {S_IRUSR, S_IWUSR, S_IXUSR};
    int group_permissions_values[3] = {S_IRGRP, S_IWGRP, S_IXGRP};
    int other_permissions_values[3] = {S_IROTH, S_IWOTH, S_IXOTH};
    char user_text[50], group_text[50], others_text[50];
    char encodings[] = "RWX";

    // Transform to RWX string for each category
    for(int idx = 0; idx < 3; ++idx) {
        user_permissions[idx] = (user_permissions_values[idx] & mode) ? encodings[idx] : '-';
        group_permissions[idx] = (group_permissions_values[idx] & mode) ? encodings[idx] : '-';
        other_permissions[idx] = (other_permissions_values[idx] & mode) ? encodings[idx] : '-';
    }   

    // Get the correct text for the type of file
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

    // Write user permissions
    write_descriptor_value_to_file(fd_statistics, user_text, user_permissions);
    write_descriptor_value_to_file(fd_statistics, group_text, group_permissions);
    write_descriptor_value_to_file(fd_statistics, others_text, other_permissions);
}


// Write bmp height width to file
void write_bmp_sizes(int fd_where, const char *bmp_file_path) {
    // Open file
    int fd_bmp = 0;
    if((fd_bmp = open(bmp_file_path, O_RDONLY)) == ERROR) {
        throw_error("");
    }

    // Get values
    char labels[2][10] = {"height", "width"};
    uint32_t values[2] = {
        get_bmp_size_value(fd_bmp, HEIGHT_POSITION),
        get_bmp_size_value(fd_bmp, WIDTH_POSITION),
    };

    // Write values
    char string_number[12], message[20];
    for(int idx = 0; idx < 2; ++idx) {
        sprintf(string_number, "%u", values[idx]);
        sprintf(message, "%s: ", labels[idx]);

        write_descriptor_value_to_file(fd_where, message, string_number);
    }
}

void write_for_regular_file(const char *file_path, int fd_statistics, struct stat statistics) {
    write_file_name(fd_statistics, "file name: ", file_path);
    write_size(fd_statistics, "size: ", statistics.st_size);
    write_user_id(fd_statistics, statistics.st_uid);
    write_modified_date(fd_statistics, statistics.st_mtim);
    write_number_hardlinks(fd_statistics, statistics.st_nlink);
    write_permissions(fd_statistics, statistics.st_mode);
}

void write_for_bmp_file(const char *file_path, int fd_statistics, struct stat statistics) {
    write_file_name(fd_statistics, "file name: ", file_path);
    write_size(fd_statistics, "size: ", statistics.st_size);
    write_bmp_sizes(fd_statistics, file_path);
    write_user_id(fd_statistics, statistics.st_uid);
    write_modified_date(fd_statistics, statistics.st_mtim);
    write_number_hardlinks(fd_statistics, statistics.st_nlink);
    write_permissions(fd_statistics, statistics.st_mode);
}

void write_for_directory_file(const char *file_path, int fd_statistics, struct stat statistics) {
    write_file_name(fd_statistics, "directory name: ", file_path);
    write_user_id(fd_statistics, statistics.st_uid);
    write_permissions(fd_statistics, statistics.st_mode);
}

void write_for_symlink_file(const char *file_path, int fd_statistics, struct stat statistics) {
    mode_t link_mode = statistics.st_mode;

    write_file_name(fd_statistics, "symlink name: ", file_path);
    write_size(fd_statistics, "link size: ", statistics.st_size);

    if(stat(file_path, &statistics) == -1) {
        throw_error("");
    }

    write_size(fd_statistics, "file size: ", statistics.st_size);
    write_permissions(fd_statistics, link_mode);
}

void do_writing(const char *file_path, const char* statistics_file_path, void(*writer)(const char*, int, struct stat)) {
    struct stat statistics = get_statistics(file_path);
    int fd_statistics = get_statistics_file_descriptor(statistics_file_path);

    writer(file_path, fd_statistics, statistics);

    close(fd_statistics);
} 

// Main functions

void write_statistics_regular_file(const char *file_path, const char *statistics_file_path) {
    do_writing(file_path, statistics_file_path, write_for_regular_file);
}

void write_statistics_bmp_file(const char *file_path, const char *statistics_file_path) {
    do_writing(file_path, statistics_file_path, write_for_bmp_file);
}

void write_statistics_directory_file(const char *file_path, const char *statistics_file_path) {
    do_writing(file_path, statistics_file_path, write_for_directory_file);
}

void write_statistics_symlink_file(const char *file_path, const char *statistics_file_path) {
    do_writing(file_path, statistics_file_path, write_for_symlink_file);
}