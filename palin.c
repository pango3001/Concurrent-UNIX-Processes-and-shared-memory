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

void isPalindrome(char str[]);

int main(int argc, char ** argv){
	
	return 0;
}


void isPalindrome(char str[]) {
	int l = 0;
	int h = strlen(str) - 1;
	while (h > l){
        	if(str[l++] != str[h--]){
			printf("%s is Not Palindrome", str); 
			return;
		}
	}
	printf("%s is palindrome", str); 
}

