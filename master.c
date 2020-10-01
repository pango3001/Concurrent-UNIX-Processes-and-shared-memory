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

//=========== MAIN ============
int main(int argc, char **argv){
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
}
