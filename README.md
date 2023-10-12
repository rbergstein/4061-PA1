# 4061 Project 1

## PA Group 98 members

Ryan Bergstein - bergs643

Marwa Osman - osman320

Basma Elaraby - elara006

## CSE computer used

csel-kh1250-02.cselabs.umn.edu

## Changes to Makefile

Added an "-lm" to link with the math library correctly.

## Individual Contributions: 

Ryan: Created a group github repository to collectively work out of and set up the project files and README document. Started work on the logic for 'utils.c'. Wrote the code to create the initial child process for our merkle tree in merkle.c. Also wrote the logic for the leaf nodes in child_process.c and debugged most of the issues with the trees creation and creaetion of the .out files.

Marwa: Polished off the logic for 'utils.c' and created the logic checks needed in 'merkle.c'. Wrote the logic to retrieve the two hashes from the two child processeses and output the computed hashes. Did final debugging for child_process.c

Basma: Did the final debugging for the 'utils.c' file, and wrote the plan for Markle Tree. Did final debugging for child_process.c. 

## Design of Merkle Tree:

Our process tree is created first with the creation of a root process in merkle.c, which when run calls child_process.c. child_process.c then runs recursively until there are N leaf processes using fork(). Our nodes pass on ID's so that when the tree has finished creation, the leaf nodes hash to their respective .out file (with the help of the provided functions given in hash.h), which is then able to be visually seen in visualization.txt.
