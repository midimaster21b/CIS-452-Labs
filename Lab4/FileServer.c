/*******************************************************
 * File: FileServer.c
 * Date: 2018-09-23
 * Authors: Joshua Edgcombe <joshedgcombe@gmail.com>
 *          Patton Finley <finleyp@mail.gvsu.edu>
 * Description: This program simulates a multi-threaded
 *              file server.
 *******************************************************/

#include <stdio.h>
#include <pthread.h>

#define MAX_FILENAME_LENGTH 100

// Shared data values for retrieval averaging
long int num_retrievals = 0;
double running_avg = 0;
pthread_mutex_t avg_lock;

int retrieve_file(char *filename);

int main() {

  // Initialize variables
  int status;
  char file_to_retrieve[MAX_FILENAME_LENGTH];

  // Greet user
  printf("Hi, Welcome To CIS452 File Retrieval System!\n");

  // Main program loop
  while(1) {

    // Prompt user for a file
    printf("Please enter the file you would like to retrieve: ");

    // Get their desired filename
    // NOTE: This does not retrieve use the return value of fgets
    //       which can indicate if an error occurred.
    fgets(file_to_retrieve, MAX_FILENAME_LENGTH, stdin);

    printf("File request: %s\n", file_to_retrieve);
  }

  return 0;
}

int retrieve_file(char *filename) {

}
