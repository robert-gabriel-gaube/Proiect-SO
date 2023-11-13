#include "process_directory.h"
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

// Returns the number of lines written to the output file
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

bool process_dir(const char *dir_path, const char *output_dir) {
    DIR* directory = opendir(dir_path);
    if(directory == NULL) {
        return false;
    }

    struct dirent* entry = NULL;
    int status = 0;

    while((entry = readdir(directory)) != NULL) {
        int pid = 0;
        if((pid = fork()) < 0) {
            closedir(directory);
            return false;
        }
        if(pid == 0) {
            FILETYPE filetype = do_statistics(dir_path, output_dir, entry->d_name);
            exit(number_of_statistics_lines(filetype));
        }
        wait(&status);
    }
    if(closedir(directory) == -1){
         return false;
    }
    return true;
}