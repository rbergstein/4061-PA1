#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>

// Create N files and distribute the data from the input file evenly among them
// See section 3.1 of the project writeup for important implementation details
void partition_file_data(char *input_file, int n, char *blocks_folder) {
    // Hint: Use fseek() and ftell() to determine the size of the file

    // VALIDATE INPUT (make sure file is real and n is a power of 2)
    FILE *Ifile = fopen(input_file, "r"); // open the input file
    if (Ifile == NULL) {
        perror("Cannot open file\n");
        exit(-1);
    }

    //check n power of 2

    fseek(Ifile, 0, SEEK_END); // set the pointer to the end of the file

    int Ifile_length = ftell(Ifile); // look at our pointer to see how big the file is
    int size_nminus1 = floor(Ifile_length / n);  // size of 0 to n - 1 files in bytes
    int size_lastfile = size_nminus1 + (Ifile_length % n); // nth file size in bytes

    rewind(Ifile); // set back to start
    char path[50];

    // mkdir("home/4061-Project-1/output/blocks", O_RDWR | O_APPEND | O_CREAT);

    for (int i = 0; i < (n - 1); i++) {   //covers 0 to n - 2 files
        sprintf(path, "%s/%d.txt", blocks_folder, i); // unique path of file

        FILE *nminus1_files = fopen(path, "wb"); // creates the file in correct directory

        char buf[size_nminus1];
        fread(buf, 1, size_nminus1, Ifile);
        fwrite(buf, 1, size_nminus1, nminus1_files);

        fclose(nminus1_files);
    }
    // need one more to cover n - 1 file
    sprintf(path, "%s/%d.txt", blocks_folder, (n - 1));

    FILE *last_file = fopen(path, "wb"); // last file in the directory

    char buf[size_lastfile];

    fread(buf, 1, size_lastfile, Ifile);
    fwrite(buf, 1, size_lastfile, last_file);

    fclose(Ifile);
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
