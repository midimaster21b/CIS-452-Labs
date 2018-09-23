/*******************************************************
 * File: FileServer.c
 * Date: 2018-09-23
 * Authors: Joshua Edgcombe <joshedgcombe@gmail.com>,
 *          Patton Finley   <finleyp@mail.gvsu.edu>
 * Description: This program simulates a multi-threaded
 *              file server.
 *******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

// Program definitions
#define MAX_FILENAME_LENGTH 100
#define MAX_WORKER_THREADS  10

// Shared data values for retrieval averaging
long int num_retrievals = 0;
double running_avg = 0;
pthread_mutex_t avg_lock;

void *retrieve_file(void *filename);

int main() {

  // Seed the random number generator with the current time
  srand(time(NULL));

  // Initialize variables
  // Error handling variables
  int status;

  // Child thread variables
  int child_iterator = 0;
  pthread_t children[MAX_WORKER_THREADS];

  // String to handle user input
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

    // Spin off a new thread to handle the request
    if((status = pthread_create(&children[child_iterator], NULL, retrieve_file, file_to_retrieve)) != 0) {
      fprintf (stderr, "Thread creation error %d: %s\n", status, strerror(status));
      exit(1);
    }
  }

  return 0;
}

void *retrieve_file(void *filename) {
  char thread_filename[MAX_FILENAME_LENGTH] = {0};
  char *status;
  int wait_time = 0;

  // Copy the filename supplied to a separate memory location to prevent it from being
  // overwritten by a second request.
  // NOTE: Return value not used
  // NOTE: Assumes string passed is appropriately sized and
  //       handled to prevent a buffer overrun here.
  strcpy(thread_filename, filename);

  // Detach the worker thread from the dispatcher
  pthread_detach(pthread_self());

  // 20% Retrieve from hard disk
  if(rand() % 10 < 2) {
    wait_time = 7 + (rand() % 4);
  }

  // 80% Retrieve from cache
  else {
    wait_time = 1;
  }

  // Wait for file retrieval
  sleep(wait_time);

  // Print the file retrieved and time taken
  printf("File retrieved(%ds): %s\n", wait_time, thread_filename);
}
