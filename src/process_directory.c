#include "process_directory.h"
#include "process_file.h"
#include "file_type.h"
#include "generals.h"
#include <sys/wait.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

#define NUMBER_PROCESSES_BMP 2
#define NUMBER_PROCESSES_REGULAR 2
#define NUMBER_PROCESSES_DIRECTORY 1
#define NUMBER_PROCESSES_SYMLINK 1

// Main functions

void process_entry(int *number_processes, const char *entry_path, const char *statistics_file_path, char c) {
    if(is_regular_file(entry_path)) {
        if(is_bmp_file(entry_path)) {
            process_bmp_file(entry_path, statistics_file_path);
            *number_processes = *number_processes + NUMBER_PROCESSES_BMP;
        }
        else {
            process_regular_file(entry_path, statistics_file_path, c);
            *number_processes = *number_processes + NUMBER_PROCESSES_REGULAR;
        }
    }
    else if(is_directory(entry_path)) {
        process_directory_file(entry_path, statistics_file_path);
        *number_processes = *number_processes + NUMBER_PROCESSES_DIRECTORY;
    }
    else if(is_symlink(entry_path)) {
        process_symlink_file(entry_path, statistics_file_path);
        *number_processes = *number_processes + NUMBER_PROCESSES_SYMLINK;
    }
}

void wait_processes(int number_processes) {
    int status;
    while(number_processes--) {
        int child_pid = 0;
        if((child_pid = wait(&status)) == ERROR) {
            throw_error("");
        }
        if(WIFEXITED(status)) {
            printf("Process with pid: %d ended with exit status: %d\n", child_pid, WEXITSTATUS(status));
        }
        else{
            throw_error("Process ended unexpectedly");
        }
    }
}

void process_directory(const char* input_directory_path, const char* output_directory_path, char c) {
    // Open input directory
    DIR* input_directory = opendir(input_directory_path);
    if(input_directory == NULL) {
        perror("");
        exit(ERROR);
    }

    // Process directory entries
    char entry_path[500], statistics_file_path[500];
    struct dirent* entry = NULL;
    int number_processes = 0;

    while((entry = readdir(input_directory)) != NULL) {
        // Build paths
        sprintf(entry_path, "%s%s", input_directory_path, entry->d_name);
        sprintf(statistics_file_path, "%s%s-statistics.txt", output_directory_path, entry->d_name);

        process_entry(&number_processes, entry_path, statistics_file_path, c);
    }

    wait_processes(number_processes);
}