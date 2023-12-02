#include "grayscale_filter.h"
#include "process_file.h"
#include "statistics.h"
#include "generals.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define READ_END 0
#define WRITE_END 1

// Auxiliary

void create_pipe(int fd_pipe[2]) {
    if(pipe(fd_pipe) == ERROR) {
        throw_error("");
    }
}

void close_pipe(int fd_pipe[2]) {
    close(fd_pipe[READ_END]);
    close(fd_pipe[WRITE_END]);
}

void redirect_to_pipe(int pipe_end) {
    int newfd = 0;
    if((newfd = dup2(pipe_end, 1)) == ERROR) {
        close(pipe_end);
        throw_error("Error with dup2");
    }
}

void read_from_pipe(int pipe_end, char *buffer) {
    int char_read = 0;
    if((char_read = read(pipe_end, buffer, 512)) == ERROR) {
        throw_error("Pipe read went wrong");
    }
    else {
        buffer[char_read] = 0;
    }
}


void write_statistics_and_redirect_content(const char *file_path, const char *statistics_file_path, int pipe_child_child[2]) {
    write_statistics_regular_file(file_path, statistics_file_path);

    // Redirect content
    close(pipe_child_child[READ_END]);

    redirect_to_pipe(pipe_child_child[WRITE_END]);

    // 'Generate' file content
    execlp("cat", "cat", file_path, NULL);

    perror("Error with execlp");
    close(pipe_child_child[WRITE_END]);
}

void run_sentence_counter(const char *file_content, char c) {
    char command[1000];
    sprintf(command, "bash sentence-counter.sh %c", c);

    // Open pipe to bash script input
    FILE *pipe = NULL;
    if((pipe = popen(command, "w")) == NULL) {
        throw_error("");
    }

    // Feed file content to bash script
    fprintf(pipe, "%s", file_content);

    if(pclose(pipe) == -1) {
        throw_error("");
    }
}

void send_good_sentences(int pipe_child_child[2], int pipe_child_parent[2], char c) {
    close(pipe_child_child[WRITE_END]);
    close(pipe_child_parent[READ_END]);

    // Read file content
    char file_content[530]; 
    read_from_pipe(pipe_child_child[READ_END], file_content);
    close(pipe_child_child[READ_END]);

    // Send good sentence count
    redirect_to_pipe(pipe_child_parent[WRITE_END]);    
    run_sentence_counter(file_content, c);

    close(pipe_child_parent[WRITE_END]);
}

// Main functions

void process_regular_file(const char *file_path, const char *statistics_file_path, char c) {
    int pipe_child_child[2], pipe_child_parent[2];

    // Write statistics and redirect content process
    create_pipe(pipe_child_child); 

    int pid_write_statistics = create_process();
    if(pid_write_statistics == 0) {
        write_statistics_and_redirect_content(file_path, statistics_file_path, pipe_child_child);
        exit(ERROR);
    }

    // Get good sentences process
    create_pipe(pipe_child_parent);

    int pid_good_sentences = create_process();
    if(pid_good_sentences == 0) {
        send_good_sentences(pipe_child_child, pipe_child_parent, c);
        exit(ERROR);
    }

    // Read good sentences
    close(pipe_child_parent[WRITE_END]);

    char number[530];
    read_from_pipe(pipe_child_parent[READ_END], number);
    number[strlen(number) - 1] = 0; // Replace \n from the end
    
    printf("There were identified in total %s correct sentences that contain the character %c.\n", number, c);

    close(pipe_child_parent[READ_END]);
    close_pipe(pipe_child_child);
}

void process_bmp_file(const char *file_path, const char *statistics_file_path) {
    // Write statistics process
    int pid_write_statistics = create_process();

    if(pid_write_statistics == 0) {
        write_statistics_bmp_file(file_path, statistics_file_path);
        exit(10); // 10 lines written to statistics
    }

    // Apply grayscale filter process
    int pid_grayscale = create_process();

    if(pid_grayscale == 0) {
        apply_grayscale_filter(file_path);
        exit(0);
    }
}

void process_directory_file(const char *file_path, const char *statistics_file_path) {
    // Write statistics process
    int pid_write_statistics = create_process();

    if(pid_write_statistics == 0) {
        write_statistics_directory_file(file_path, statistics_file_path);
        exit(5); // 5 lines written to statistics
    }
}

void process_symlink_file(const char *file_path, const char *statistics_file_path) {
    // Write statistics process
    int pid_write_statistics = create_process();

    if(pid_write_statistics == 0) {
        write_statistics_symlink_file(file_path, statistics_file_path);
        exit(6); // 6 lines written to statistics
    }
}