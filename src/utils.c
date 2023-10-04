#include "utils.h"
#include <math.h>

// Create N files and distribute the data from the input file evenly among them
// See section 3.1 of the project writeup for important implementation details
void partition_file_data(char *input_file, int n, char *blocks_folder) {
    // Hint: Use fseek() and ftell() to determine the size of the file
    char file_name[PATH_MAX];

    FILE *orig_file = fopen(input_file, "r");
    // do error check
    if (orig_file == NULL) {
        perror("Error opening the file");
        exit(-1);
    }


    //determine size of file
    fseek(orig_file, 0, SEEK_END);
    long file_size = ftell(orig_file);

    //determine how many bytes to read/write
    int block_size = floor(file_size / n);
    int last_block_size = floor(file_size / n) + (file_size % n);

    fseek(orig_file, 0, SEEK_SET);
    for (int i=0; i < n-1; i++) {
        sprintf(file_name, "%s/%d.txt", blocks_folder, i);
        FILE *new_file = fopen(file_name, "w+");

        // block_size should be 
        char buffer[block_size];
        fread(buffer, 1, block_size, orig_file);
        fwrite(buffer, 1, block_size, new_file);

        // fseek(orig_file, (block_size * (i + 1)), SEEK_SET);

        fclose(new_file);
    }
    
    //read and write to last file
    char buffer[last_block_size];
    sprintf(file_name, "%s/%d.txt", blocks_folder, n-1);
    FILE *new_file = fopen(file_name, "w+");
    
    fread(buffer, 1, last_block_size, orig_file);
    fwrite(buffer, 1, last_block_size, new_file);
    fclose(new_file);


    fclose(orig_file);
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
