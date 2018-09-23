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
#include <signal.h>

// Program definitions
#define MAX_FILENAME_LENGTH 100
#define MAX_WORKER_THREADS  10

// Shared data values for retrieval averaging
long int num_retrievals = 0;
double running_avg = 0;
pthread_mutex_t avg_lock;

// Track the number of requests received
long int num_requests = 0;

void *retrieve_file(void *filename);
void update_running_avg(int wait_time);
void process_kill_handler(int signal_id);

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

  // Initialize mutex
  if((status = pthread_mutex_init(&avg_lock, NULL)) != 0) {
    printf("Mutex init error %d: %s\n", status, strerror(status));
    exit(1);
  }

  // Register signal handler for ^C
  signal(SIGINT, process_kill_handler);

  // Main program loop
  while(1) {

    // Prompt user for a file
    printf("Please enter the file you would like to retrieve: ");

    // Get their desired filename
    // NOTE: This does not retrieve use the return value of fgets
    //       which can indicate if an error occurred.
    fgets(file_to_retrieve, MAX_FILENAME_LENGTH, stdin);

    // Increment the total number of requests received
    num_requests++;

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

  // Update current average
  update_running_avg(wait_time);

  // Print the file retrieved and time taken
  printf("File retrieved(%ds, %lfs avg): %s", wait_time, running_avg, thread_filename);

  return 0;
}

void update_running_avg(int wait_time) {

  // Lock the mutex
  pthread_mutex_lock(&avg_lock);

  // Increment the total number of retrievals
  num_retrievals++;

  // Recalculate the running average
  running_avg = (((num_retrievals - (double)1.0) / (num_retrievals)) * running_avg) + (((double)1.0 / num_retrievals) * wait_time);

  // Release the mutex
  pthread_mutex_unlock(&avg_lock);
}

void process_kill_handler(int signal_id) {

  // Destroy the mutex
  pthread_mutex_destroy(&avg_lock);

  // Print out the final average
  printf("\n\nNumber of files requested: %ld\n", num_requests);
  printf("Number of files retrieved: %ld\n", num_retrievals);
  if(num_retrievals != 0) {
    printf("Average retrieval time: %lf\n", running_avg);
  }
  else {
    printf("Average retrieval time: Not Available\n");
  }
    
  printf("Goodbye!\n");

  // Exit the program
  exit(1);
}
