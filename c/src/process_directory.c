#include "process_directory.h"
#include "bmp_transform.h"
#include "statistics.h"
#include "generals.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

FILETYPE do_statistics(const char* source_dir_path, 
                       const char* destination_dir_path, 
                       const char* entry_name) {
    
    FILETYPE filetype = ERROR;

    char output_file[500], file_path[500];
    sprintf(file_path, "%s%s", source_dir_path, entry_name);
    sprintf(output_file, "%s%s-statistics.txt", destination_dir_path, entry_name);

    int fd_statistics = creat(output_file, S_IRUSR | S_IWUSR);
    if(fd_statistics == -1) {
        return ERROR;
    }
    if((filetype = write_statistics(fd_statistics, file_path)) == ERROR) {
        close(fd_statistics);
        return ERROR;
    }

    if(close(fd_statistics) == -1) {
        return ERROR;
    }
    return filetype;
}

// Returns the number of lines written to the output file.
int number_of_statistics_lines(FILETYPE filetype) {
    switch(filetype) {
        case BMP_FILE: return 10;
        case REGULAR_FILE: return 8;
        case SYMLINK: return 6;
        case DIRECTORY: return 5;
        case OTHER: return 0;
        case ERROR: return -1;
    }
    return -2;
}

bool transform_grayscale(const char *dir_path, const char *entry_name) {
    int pid_grayscale = 0;

    if((pid_grayscale = fork()) < 0) {
        PRINT_DEBUG("ERROR", "Process failed to create");
        return false;
    }
    if(pid_grayscale == 0) {
        char file_path[500];
        sprintf(file_path, "%s%s", dir_path, entry_name);
        if(!grayscale_filter(file_path)) return false;
        exit(0);
    }
    return true;
}

bool wait_processes(int number_processes) {
    int status;
    while(number_processes--) {
        int child_pid = 0;
        if((child_pid = wait(&status)) == -1) {
            return false;
        }
        if(WIFEXITED(status)) {
            printf("Process with pid: %d ended with exit status: %d\n", child_pid, WEXITSTATUS(status));
        }
        else{
            printf("Process ended unexpectedly\n");
            return false;
        }
    }
    return true;
}

bool statistics_write(const char* dir_path, const char *output_dir, const char* entry_name) {
    int pid;
    if((pid = fork()) < 0) {
        return false;
    }
    if(pid == 0) {
        FILETYPE filetype = do_statistics(dir_path, output_dir, entry_name);
        exit(number_of_statistics_lines(filetype));
    }
    return true;
}

bool process_dir(const char *dir_path, const char *output_dir) {
    DIR* directory = opendir(dir_path);
    if(directory == NULL) {
        return false;
    }

    struct dirent* entry = NULL;
    int number_processes = 0;

    while((entry = readdir(directory)) != NULL) {
        if(is_same_extension(entry->d_name, ".bmp")) {
            PRINT_DEBUG("DEBUG", "Trying to create process");

            ++number_processes;
            if(!transform_grayscale(dir_path, entry->d_name)) return false;
        }
        PRINT_DEBUG("DEBUG", "Trying to create process");

        ++number_processes;
        if(!statistics_write(dir_path, output_dir, entry->d_name)) return false;
        
    }
    if(!wait_processes(number_processes)) {
        closedir(directory);
        return false;
    }

    if(closedir(directory) == -1){
         return false;
    }
    return true;
}