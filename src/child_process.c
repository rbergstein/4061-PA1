#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "hash.h"
#include "sha256.h"

#define PATH_MAX 1024

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: ./child_process <blocks_folder> <hashes_folder> <N> <child_id>\n");
        return 1;
    }

    int N = atoi(argv[3]);
    int currentID = atoi(argv[4]);
    // TODO: If the current process is a leaf process, read in the associated block file 
    // and compute the hash of the block.
    if (currentID >= (N - 1) && currentID <= ((2 * N) - 2)) { // loop through leaves
        char target_name[PATH_MAX];
        char hash_buffer[SHA256_BLOCK_SIZE * 2 + 1];
        
        sprintf(target_name, "%s/%d.txt", argv[1], currentID - 1);

        FILE *target_block = fopen(target_name, "r");

        hash_data_block(hash_buffer, target_name); //  from hash.h

        fclose(target_block);
    }
    // TODO: If the current process is not a leaf process, spawn two child processes using  
    // exec() and ./child_process. 
    else {
        int childpid1 = fork();

        if (childpid1 == 0) { // child 
            char childID1[20];
            sprintf(childID1, "%d", (atoi(argv[3]) - 1)); // child 1 ID (maybe?)

            // char *array[] = {blocks folder, hashes folder, N, ID, NULL}
            char *child_arr1[] = {argv[1], argv[2], argv[3], childID1, NULL};
            execv("./child_process", child_arr1);
        } else{
            int childpid2 = fork();

            if (childpid2 == 0) {
                char childID2[20];
                sprintf(childID2, "%d", (atoi(argv[3]) - 2)); // child 2 ID (maybe?)

                // char *array[] = {blocks folder, hashes folder, N, ID, NULL}
                char *child_arr2[] = {argv[1], argv[2], argv[3], childID2, NULL};
                execv("./child_process", child_arr2);
            } else { // TODO: Wait for the two child processes to finish
            waitpid(childpid1, NULL, 0);
            waitpid(childpid2, NULL, 0);
            }
        }
    }
    // TODO: Retrieve the two hashes from the two child processes from output/hashes/
    // and compute and output the hash of the concatenation of the two hashes.
    char left_hash[SHA256_BLOCK_SIZE * 2 + 1];
    char right_hash[SHA256_BLOCK_SIZE * 2 + 1];
    char result_hash[SHA256_BLOCK_SIZE * 2 + 1];
    compute_dual_hash(result_hash, left_hash, right_hash);
    // this is all just from hash.h
}

