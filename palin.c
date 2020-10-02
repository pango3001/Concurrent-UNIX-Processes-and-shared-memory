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

#define PERMS (S_IRUSR | S_IWUSR)

void signal_handle(int signal);
int free_shared_mem();

bool isPalindrome(char str[]);


typedef struct{
	int id;
        int key_t;  //key_t key;
        char strings[64][64]; // array of strings
} shared_memory;
shared_memory *ptr; // pointer for the shared memory
int shm_id;


int main(int argc, char ** argv){
	
	printf("test\n");
	signal(SIGINT,signal_handle);
	int id = atoi(argv[1]);

	 	
	unsigned int key = ftok("./master", 'a');	

	shm_id = shmget(key, sizeof(shared_memory), PERMS | IPC_CREAT);
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

	
	//printf("child argument: %d ,string %s",id, ptr->strings[id]);

	bool palin = isPalindrome(ptr->strings[id]);



	time_t t;		
	srand((unsigned) time(&t)); // seed for random sleep time
	sleep(rand() % (3)); // sleep between 0-2 seconds

	if(palin){
		printf("%s is palin\n", ptr->strings[id]);
	}
	else{
		printf("%s is not\n", ptr->strings[id]);
	}
	

	
	
	// writing to output files
	FILE *file_a = fopen(palin ? "palin.out" : "nopalin.out", "a+"); // a+ appends
	if(file_a == NULL){
		perror("FILE ERROR");
	}
	fprintf(file_a, "%s\n", ptr->strings[id]);
	
	pid_t pid = getpid();
	fclose(file_a); // closes file_a
	
	FILE *file_b = fopen("output.log", "a+");
        if(file_b == NULL){
              perror("FILE ERROR");
        }
	fprintf(file_b, "PID: %d, Index: %d, String %s\n", pid, ptr->id, ptr->strings[id]); 

	fclose(file_b);
	
	





	// FOR TESTING SIGNAL CATCH INFINTIE LOOP
	//while(true){
	//	printf("\ntesting... ");
	//	sleep(1);
	//}


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

void signal_handle(int signal){
	printf("\nMessage: Ctrl+c in child caught!\n");
	exit(1);
}

int free_shared_mem(){
	//detach from shared memory
	int detach = shmdt(ptr);
        if (detach == -1){
                perror("Failed to detach shared memory segment");
                return 1;
        }

        int delete_mem = shmctl(shm_id, IPC_RMID, NULL);
        if (delete_mem == -1){
                perror("Failed to remove shared memory segment");
                return 1;
        }
	return 1;
}
