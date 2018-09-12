/******************************************************
 * Title: shell.c
 * Authors: Patton Finley <finleyp@mail.gvsu.edu>
 *          Joshua Edgcombe <joshedgcombe@gmail.com>
 *
 * Description: Simple shell program for lab 2 of
 *              CIS 452: Operating System Concepts
 ******************************************************/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 256


int main(int argc, char*argv[])
{
	puts("This is a shell program for UNIX systems. Please insert UNIX commands: ");
	
	char buffer[SIZE];
	char* temp;
	char* vector[SIZE];
	int spot;
	pid_t pid, child;
	int status;
	while(1){
		fgets(buffer, SIZE, stdin);
		for(spot = 0; buffer[spot]!='\0'; spot = spot + 1){
			vector[spot] = strtok(buffer, " ");
		}
		if(!strcmp(vector[0], "quit")){
			puts("Exiting");
			exit(0);
		}
		if((pid = fork()) < 0){
			perror("fork failed");
			exit(0);
		}
		else if(pid){
			waitpid(-1, &status, WUNTRACED);
		}
		else{
			execvp(vector[0],vector);
			exit(0);
		}
	}
}
