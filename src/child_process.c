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
    char *block_folder = argv[1];
    char *hash_folder  = argv[2];

    

    //int fileID = currentID + (N - 1);
    // TODO: If the current process is a leaf process, read in the associated block file 
    // and compute the hash of the block.
    
    if (currentID >= N -1){
        char target_name[PATH_MAX];
        char hash_buffer[SHA256_BLOCK_SIZE * 2 + 1];
        sprintf(target_name, "%s/%d.txt", block_folder, currentID - N + 1);  

        hash_data_block(hash_buffer, target_name); //  from hash.h
       
        char target_name1[PATH_MAX];

        sprintf(target_name1, "%s/%d.out", hash_folder, currentID);
        FILE *target_block = fopen(target_name1, "w");

        if (target_block == NULL) {
            printf("Cannot open file in child_process\n");
            exit(-1);
        }

       fprintf(target_block, "%s", hash_buffer);
       fclose(target_block);
       exit(0);

       
    }
    // TODO: If the current process is not a leaf process, spawn two child processes using  
    // exec() and ./child_process. 
    char childID1_str[100];
    sprintf(childID1_str, "%d", 2* currentID + 1);
    char childID2_str[100];
    sprintf(childID2_str, "%d", 2*currentID + 2);

    
    pid_t childpid1 ,childpid2;
    childpid1 = fork();

    if (childpid1 == 0) { // child 
        char *child_arr1[] = {"./child_process", block_folder, hash_folder, argv[3], childID1_str, NULL};
        execvp("./child_process", child_arr1);
        exit(0);
            
    } else { // parent
        childpid2 = fork();
        if (childpid2 == 0) {
            char *child_arr2[] = {"./child_process", block_folder, hash_folder, argv[3], childID2_str, NULL};
            if(execvp("./child_process", child_arr2) < 0)
            {
                printf("ERRROR");
            };
            exit(0);
            

        } 
        // TODO: Wait for the two child processes to finish
    }

    if(waitpid(childpid1, NULL, 0) == -1)
    {
        printf("ERROR 122\n");
    };
    if(waitpid(childpid2, NULL, 0) == -1)
    {
        printf("ERROR 122\n");
    };

    // TODO: Retrieve the two hashes from the two child processes from output/hashes/
    // and compute and output the hash of the concatenation of the two hashes.

    // moved down here so the hashes are computed AFTER the child processes have completed
    char left_hash[SHA256_BLOCK_SIZE * 2 + 1];
    char right_hash[SHA256_BLOCK_SIZE * 2 + 1];
    char result_hash[SHA256_BLOCK_SIZE * 2 + 1];

    char left_buf[PATH_MAX];
    char right_buf[PATH_MAX];
    char result_buf[PATH_MAX];

    sprintf(left_buf, "%s/%d.out", hash_folder, 2 * currentID + 1);
    sprintf(right_buf, "%s/%d.out", hash_folder, 2 * currentID + 2);
    sprintf(result_buf, "%s/%d.out", hash_folder, currentID);
    
    FILE *fp1 = fopen(left_buf, "r");
    FILE *fp2 = fopen(right_buf, "r");
    FILE *fp3 = fopen(result_buf, "w");

    if (fp1 == NULL) {
        printf("Cannot open fp1 %s\n", left_buf);
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
    fscanf(fp1, "%s", left_hash);
    fscanf(fp2, "%s", right_hash);
    //fread(left_hash, sizeof(left_hash), (SHA256_BLOCK_SIZE * 2 + 1), fp1);
    //fread(right_hash, sizeof(right_hash), (SHA256_BLOCK_SIZE * 2 + 1), fp2);
    
    compute_dual_hash(result_hash, left_hash, right_hash);

    //fwrite(result_hash, (SHA256_BLOCK_SIZE * 2 + 1), sizeof(result_hash), fp3);
    fprintf(fp3, "%s", result_hash);

    //fclose(fp1);
    //fclose(fp2);
    //fclose(fp3);

    exit(0);

}


