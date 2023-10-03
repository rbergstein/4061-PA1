#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

// Create N files and distribute the data from the input file evenly among them
// See section 3.1 of the project writeup for important implementation details
void partition_file_data(char *input_file, int n, char *blocks_folder) {
    // Hint: Use fseek() and ftell() to determine the size of the file
    FILE *file = fopen(input_file, "r"); // open the input file
    if (file == NULL) {
        perror("Cannot open file\n");
        exit(-1);
    }

    fseek(file, 0, SEEK_END); // set the pointer to the end of the file

    int file_length = ftell(file); // look at our pointer to see how big the file is
    int size_nminus1 = floor(file_length / n);  // size of 0 to n - 1 files in bytes
    int size_lastfile = floor(file_length / n) + (file_length % n); // nth file size in bytes

    for (int i = 0; i < (n - 1); i++) {   //covers 0 to n - 2 files
        char path[50];
        sprintf(path, "output/blocks/%s.txt", i); // unique path of file

        int file = open(path, O_RDWR | O_APPEND | O_CREAT); // creates the file in correct directory


    }
    // need one more to cover n - 1 file
    char path[50];
    sprintf(path, "output/blocks/%s.txt", (n - 1));

    int lastfile = open(path, O_RDWR | O_APPEND | O_CREAT); // last file in the directory











    fclose(file);
}


// ##### DO NOT MODIFY THIS FUNCTION #####
void setup_output_directory(char *block_folder, char *hash_folder) {
    // Remove output directory
    pid_t pid = fork();
    if (pid == 0) {
        char *argv[] = {"rm", "-rf", "./output/", NULL};
        if (execvp(*argv, argv) < 0) {
            printf("ERROR: exec failed\n");
            exit(1);
        }
        exit(0);
    } else {
        wait(NULL);
    }

    sleep(1);

    // Creating output directory
    if (mkdir("output", 0777) < 0) {
        printf("ERROR: mkdir output failed\n");
        exit(1);
    }

    sleep(1);

    // Creating blocks directory
    if (mkdir(block_folder, 0777) < 0) {
        printf("ERROR: mkdir output/blocks failed\n");
        exit(1);
    }

    // Creating hashes directory
    if (mkdir(hash_folder, 0777) < 0) {
        printf("ERROR: mkdir output/hashes failed\n");
        exit(1);
    }
}
