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

bool process_dir(const char *dir_path, const char *output_dir) {
    DIR* directory = opendir(dir_path);
    if(directory == NULL) {
        return false;
    }

    struct dirent* entry = NULL;
    char file_path[500];
    int status;

    while((entry = readdir(directory)) != NULL) {
        sprintf(file_path, "%s%s", dir_path, entry->d_name);

        int pid = 0;
        if((pid=fork()) < 0) {
            closedir(directory);
            return false;
        }
        if(pid==0) {
            char output_file[500];
            sprintf(output_file, "%s%s-statistics.txt", output_dir, entry->d_name);

            int fd_statistics = creat(output_file, S_IRUSR | S_IWUSR);
            if(fd_statistics == -1) {
                closedir(directory);
                return false;
            }
            if(write_statistics(fd_statistics, file_path) == ERROR) {
                close(fd_statistics); closedir(directory);
                return false;
            }

            if(close(fd_statistics) == -1) {
                closedir(directory);
                return false;
            }
            exit(0);
        }
        wait(&status);
    }
    if(closedir(directory) == -1){
         return false;
    }
    return true;
}