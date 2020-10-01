//AUTHOR: Jesse McCarville-Schueths
//COURSE: CS 4760
//DATE: SEPT 28 2020
//FILENAME: master.c
//
//DESCRIPTION: working with Concurrent UNIX Processes and shared memory
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define PERM (S_IRUSR | S_IWUSR)

//=========== MAIN ============
int main(int argc, char **argv){
	unsigned int key = 63376;
	
	unsigned int options;
	unsigned int max_childs_master = 4; // maximum total of child processes master will ever create
	unsigned int max_total_childs = 2; // number of children allowed to exist in the system at once
	unsigned int max_time = 100; //time in seconds after which the process will terminate

	struct Shared_mem{
	int id;
	int key_t;  //key_t key;
	char strings[256][256]; // array of strings
	} shared_memory;
	shared_memory* ptr;
	
	
	// returns the identifier of the System V shared memory segment associated with the value of the argument key
	int shm_id = shmget(key, sizeof(shared_memory), PERM | IPC_CREAT | IPC_EXCL);
	if (shm_id == -1) {
		perror("Failed to create shared memory segment");
		return 1;
	}
	
	// attaches the System V shared memory segment identified by shmid to the address space of the calling process
	shared_memory* ptr = (shared_memory*)shmat(shm_id, NULL, 0);
	if (ptr == (void*)-1) {
		perror("Failed to attach shared memory segment");
		return 1;
	}

	//getopts
	while((options = getopt(argc, argv, "hn:s:t:")) != -1){
		switch(options){
			case 'h':  
				printf("Usage: master [-h] [-n x] [-s x] [-t time] <infile>\n");
				printf("\t-h Print a help message or usage, and exit\n");
				printf("\t-n x Indicate the maximum total of child processes master will ever create. (Default 4)\n");
				printf("\t-s x Indicate the number of children allowed to exist in the system at the same time. (Default 2)\n");
				printf("\t-t The time in seconds after which the process will terminate, even if it has not finished. (Default 100)\n");
				return EXIT_SUCCESS;
			case 'n':
				max_childs_master = atoi(optarg); // argument replaces default
				if(max_childs_master > 20){
					max_childs_master = 20; // hard limit for total child running at once
				}
				break;
			case 's':
				max_total_childs = atoi(optarg); // argument replaces default
				break;
			case 't':
				max_time = atoi(optarg); // argument replaces default
				break;
			default:
				printf("ERROR: use option -h for help.\n");
				return EXIT_FAILURE;
		}
	}
	
	//read file and write contents to array
	FILE *fp = fopen(argv[optind], "r");
	// file read error
	if( fp == NULL ){
		perror("ERROR ");
		return(-1);
	}
	int index = 0;
	char line[256];
	while (fgets(line, sizeof(line), fp)){
		line[strlen(line) - 1] = '\0';
		strcpy(ptr->strings[index], line);
		index++;
	}
	
	fclose(fp);
	// print arguments and array from file
	index = 0;
	int size = sizeof(ptr->strings);
	while(index < size){
		printf("%s ", ptr->strings[index]);
		index;
	}
	
	return 1;
}
