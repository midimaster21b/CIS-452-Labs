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

#define SIZE 200


int main(int argc, char*argv[])
{
	puts("This is a shell program for UNIX systems. Please insert UNIX commands: ");
	//puts("Input must be less than 200 characters");
	
	char buffer[SIZE];
	char* temp;
	char vector[SIZE];
	int spot;

	fgets(buffer, SIZE, stdin);
	printf("%s%s\n","BUFFER: ", buffer);	
	temp = strtok(buffer, " ");
	spot = 0;
	while(temp != NULL){
		printf("%s%s\n","temp: ", temp);
		printf("%s%i\n", "spot: ", spot);
		strcat(vector,temp);
		//vector += temp;
		temp = strtok (NULL, " ");
		spot += 1;
	}
	printf("%s%s\n","vector: ", vector);
	puts("Thanks good bye");
	exit(0);
	

}
