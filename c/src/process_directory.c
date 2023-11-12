#include "process_directory.h"
#include "statistics.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

bool process_dir(const char *dir_path) {
    DIR* directory = opendir(dir_path);
    if(directory == NULL) {
        return false;
    }

    struct dirent* entry = NULL;
    char file_path[500];

    int fd_statistics = creat("statistics.txt", S_IRUSR | S_IWUSR);
    if(fd_statistics == -1) {
        closedir(directory);
        return false;
    }

    while((entry = readdir(directory)) != NULL) {
        sprintf(file_path, "%s%s", dir_path, entry->d_name);
        if(!write_statistics(fd_statistics, file_path)) {
            close(fd_statistics); closedir(directory);
            return false;
        }
    }

    if(close(fd_statistics) == -1) {
        closedir(directory);
        return false;
    }
    if(closedir(directory) == -1){
         return false;
    }
    return true;
}