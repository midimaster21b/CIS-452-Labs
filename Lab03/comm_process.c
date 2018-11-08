/***************************************************************
 * Title: comm_process.c
 * Authors: Joshua Edgcombe <joshedgcombe@gmail.com>
 *          Patton Finley <finleyp@mail.gvsu.edu>
 *
 * Description: A simple program showing the appropriate use of
 *              linux signals.
 **************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

// Prototypes for the signal handlers
void sigHandlerParent(int);
void sigHandlerChild(int);

// Global variables for tracking parent and child PID's
pid_t child_pid, parent_pid;

int main() {
  // Process ID variables
  child_pid = fork();

  // Random number
  srand(time(NULL));   // should only be called once
  int r = rand();

  // Child process
  if(child_pid == 0) {
    // Get parent process PID
    parent_pid = getppid();
  }

  // Error handling (child_pid < 0)
  else if(child_pid < 0){
    printf("Error: Fork failed.\n");
    return(1);
  }

  while(1) {
    // Child process
    if(child_pid == 0) {
      // Register child signal handler
      signal(SIGINT, sigHandlerChild);

      // Get a random number for the amount of time
      // for the child process to sleep for.
      r = (rand() % 4) + 1;

      // Wait some random time
      sleep(r);

      // Send a random signal to the parent
      if(rand() % 2) {
	kill(parent_pid, SIGUSR1);
      }
      else {
	kill(parent_pid, SIGUSR2);
      }
    }

    // Parent process
    else if(child_pid > 0) {
      // Register signal handlers
      signal(SIGINT, sigHandlerParent);
      signal(SIGUSR1, sigHandlerParent);
      signal(SIGUSR2, sigHandlerParent);

      // Print waiting and flush to the terminal (print immediately)
      printf("Waiting...\t\t");
      fflush(stdout);

      // Wait for interrupt to occur
      pause();
    }
  }

  return 0;
}

void sigHandlerParent(int sigNum) {

  // Siwtch on the supplied signal
  switch(sigNum) {
  case SIGINT:
    // Kill the child
    kill(child_pid, SIGINT);

    // Print we're leaving
    printf(" received. That's it, I'm shutting down...\n");

    // Exit program
    exit(0);
    break;

  case SIGUSR1:
    // Print appropriate signal to the terminal
    printf("SIGUSR1 detected...\n");
    break;

  case SIGUSR2:
    // Print appropriate signal to the terminal
    printf("SIGUSR2 detected...\n");
    break;

  default:
    // Handle unknown signals
    printf("Unknown signal detected...\n");
    break;
  }
}

// Child signal handler
void sigHandlerChild(int sigNum) {
  // On receiving a signal, exit out of process
  exit(0);
}
