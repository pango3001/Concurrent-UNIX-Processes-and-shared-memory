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

bool isPalindrome(char str[]);


typedef struct{
	int id;
        int key_t;  //key_t key;
        char strings[64][64]; // array of strings
} shared_memory;



int main(int argc, char ** argv){

	bool palin = isPalindrome(argv[1]);
		


	if(palin){
		printf("%s is palin\n", argv[1]);
	}
	else{
		printf("%s is not\n", argv[1]);
	}
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

