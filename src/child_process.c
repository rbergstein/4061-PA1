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
        exit(1);
    }

    int N = atoi(argv[3]);
    int currentID = atoi(argv[4]);

    char target_name[PATH_MAX];
    char hash_buffer[SHA256_BLOCK_SIZE * 2 + 1];

    int fileID = currentID + (N - 1);
    // TODO: If the current process is a leaf process, read in the associated block file 
    // and compute the hash of the block.
    
    sprintf(target_name, "%s/%d.out", argv[2], fileID);

    FILE *target_block = fopen(target_name, "w+");

    if (target_block == NULL) {
        printf("Cannot open file in child_process\n");
        exit(-1);
    }

    hash_data_block(hash_buffer, target_name); //  from hash.h

    fwrite(hash_buffer, 1, sizeof(hash_buffer), target_block);

    fclose(target_block);
    // TODO: If the current process is not a leaf process, spawn two child processes using  
    // exec() and ./child_process. 
    if (currentID < (N - 2)) {
        int childpid1 = fork();

        if (childpid1 == 0) { // child 
            int childID1 = currentID;
            char childID1_str[20];
            sprintf(childID1_str, "%d", childID1);

            // char *array[] = {blocks folder, hashes folder, N, ID, NULL}
            char *child_arr1[] = {"./child_process", argv[1], argv[2], argv[3], childID1_str, NULL};
            execv("./child_process", child_arr1);
            
        } else{ // parent
            int childpid2 = fork();
            if (childpid2 == 0) {
               
                int childID2 = currentID + 1;
                char childID2_str[20];
                sprintf(childID2_str, "%d", childID2);

                // char *array[] = {blocks folder, hashes folder, N, ID, NULL}
                char *child_arr2[] = {"./child_process", argv[1], argv[2], argv[3], childID2_str, NULL};
                execv("./child_process", child_arr2);
            } else { // TODO: Wait for the two child processes to finish


            waitpid(childpid1, NULL, 0);
            waitpid(childpid2, NULL, 0);

            }
        }

    }
    // TODO: Retrieve the two hashes from the two child processes from output/hashes/
    // and compute and output the hash of the concatenation of the two hashes.

    // moved down here so the hashes are computed AFTER the child processes have completed
    char left_hash[SHA256_BLOCK_SIZE * 2 + 1];
    char right_hash[SHA256_BLOCK_SIZE * 2 + 1];
    char result_hash[SHA256_BLOCK_SIZE * 2 + 1];

    char left_buf[PATH_MAX];
    char right_buf[PATH_MAX];
    char result_buf[PATH_MAX];

    sprintf(left_buf, "%s/%d.out", argv[2], fileID);
    sprintf(right_buf, "%s/%d.out", argv[2], fileID + 1);
    sprintf(result_buf, "%s/%d.out", argv[2], N);
    
    FILE *fp1 = fopen(left_buf, "w+");
    FILE *fp2 = fopen(right_buf, "w+");
    FILE *fp3 = fopen(result_buf, "w+");

    if (fp1 == NULL) {
        printf("Cannot open fp1\n");
        exit(-1);
    }
    if (fp2 == NULL) {
        printf("Cannot open fp2\n");
        exit(-1);
    }
    if (fp3 == NULL) {
        printf("Cannot open fp3\n");
        exit(-1);
    }

    fread(left_hash, sizeof(left_hash), (SHA256_BLOCK_SIZE * 2 + 1), fp1);
    fread(right_hash, sizeof(right_hash), (SHA256_BLOCK_SIZE * 2 + 1), fp2);
    
    compute_dual_hash(result_hash, left_hash, right_hash);

    fwrite(result_hash, (SHA256_BLOCK_SIZE * 2 + 1), sizeof(result_hash), fp3);

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    
}

