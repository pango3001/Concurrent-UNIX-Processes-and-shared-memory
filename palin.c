//AUTHOR: Jesse McCarville-Schueths
//COURSE: CS 4760
//DATE: SEPT 28 2020
//FILENAME: palin.c
//
//DESCRIPTION: working with Concurrent UNIX Processes and shared memory
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define PERM (S_IRUSR | S_IWUSR)

bool isPalindrome(char str[]);


typedef struct{
	int id;
        int key_t;  //key_t key;
        char strings[64][64]; // array of strings
} shared_memory;



int main(int argc, char ** argv){
	int id = atoi(argv[1]);
	shared_memory *ptr; // pointer for the shared memory

	
	unsigned int key = ftok("./master", 'a');	

	int shm_id = shmget(key, sizeof(shared_memory), PERM | IPC_CREAT);
	if (shm_id == -1) {
		perror("Failed to find shared memory segment");
		return 1;
	}
	
	// attach shared memory segment
	ptr = (shared_memory*)shmat(shm_id, NULL, 0);
	// shmat(segment_id, NULL, SHM_RDONLY) to attach to read only memory
	if (ptr == (void*)-1) {
		perror("Failed to attach to shared memory segment");
		return 1;
	}

	

	bool palin = isPalindrome(ptr->strings[id]);
		


	if(palin){
		printf("%s is palin\n", ptr->strings[id]);
	}
	else{
		printf("%s is not\n", ptr->strings[id]);
	}
	


	// writing to output files
	FILE *file = fopen(palin ? "palin.out" : "nopalin.out", "a+"); // a+ appends
	if(file == NULL){
		perror("FILE ERROR");
	}
	fprintf(file, "%s\n", ptr->strings[id]);
	fclose(file);
	
		



	return 0;
}


bool isPalindrome(char str[]) {
	bool palin = true;
	int l = 0;
	int h = strlen(str) - 1;
	while (h > l){
        	if(str[l++] != str[h--]){		 
			return false;
		}
	}
	return true; 
}

