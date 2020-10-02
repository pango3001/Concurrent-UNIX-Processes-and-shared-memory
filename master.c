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
#include <sys/ipc.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define LSIZ 128 
#define RSIZ 32
#define PERMS (S_IRUSR | S_IWUSR)

//========== PROTOTYPES ==========
void run_child(int id);
int free_shared_mem();
void signal_handle(int signal);

// structure for the shared memory
typedef struct{
	int id;
	int key_t;  //key_t key;
	char strings[64][64]; // array of strings
} shared_memory;

shared_memory* ptr; // pointer to shared memory
int shm_id; // segment id



//=========== MAIN ============
int main(int argc, char **argv){	
	signal(SIGINT, signal_handle);
	// key for shared memory
	unsigned int key = ftok("./master", 'a');
	
		
	// returns the identifier of the System V shared memory segment associated with the value of the argument key
	shm_id = shmget(key, sizeof(shared_memory), PERMS | IPC_CREAT | IPC_EXCL);
	if (shm_id == -1) {
	        perror("Failed to create shared memory segment");
        	return 1;
	}

	// attaches the System V shared memory segment identified by shmid to the address space of the calling process
	ptr = (shared_memory*) shmat(shm_id, NULL, 0);
	if (ptr == (void*)-1) {
        	perror("Failed to attach shared memory segment");
                return 1;
        }
	

	unsigned int options;
	unsigned int max_childs_master = 4; // maximum total of child processes master will ever create
	unsigned int max_total_childs = 2; // number of children allowed to exist in the system at once
	unsigned int max_time = 100; //time in seconds after which the process will terminate


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
	int i = 0;


	while(fgets(ptr->strings[i],LSIZ, fp)){
		ptr->strings[i][strlen(ptr->strings[i]) - 1] = '\0';
		i++;
	}
	int total = i;
	
	fclose(fp);


	// print arguments and array from file
	int index = 0;
	int size = sizeof(ptr->strings);
	while(index < total){
		printf("Line %d: ", (index + 1));
		printf("%s \n", ptr->strings[index]);
		index++;
	}




	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// IDK PART
	//
	//


	index = 0;
	
	//run_child(index);
	index++;

	int pid = fork();

	if(pid == 0){
		run_child(1);
		exit(1);
	}
	int status;
    	waitpid(pid, &status, 0);
	index++;
	






	free_shared_mem();

	return 1;
}


void run_child(int id){
        char buffer[256];
        sprintf(buffer, "%d", id);
        execl("./palin", "palin", buffer, (char*) NULL);
}

int free_shared_mem(){
	//detatch from the shared memory segment
	int detach = shmdt(ptr);
        if (detach == -1){
                //used for debugging
		//perror("Failed to detach shared memory segment");
                return 1;
        }

        int delete_mem = shmctl(shm_id, IPC_RMID, NULL);
        if (delete_mem == -1){
                perror("Failed to remove shared memory segment");
                return 1;
        }
	return 1;
}

void signal_handle(int signal){
	//TODO kill children
	
	free_shared_mem();
}
