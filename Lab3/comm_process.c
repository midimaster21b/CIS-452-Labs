#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include <time.h>
/* #include <stdlib.h> */

void sigHandlerParent(int);
void sigHandlerChild(int);

int main() {

  // Process ID variables
  pid_t child_pid = fork();
  pid_t parent_pid;

  // Random number
  srand(time(NULL));   // should only be called once
  int r = rand();

  // Child process
  if(child_pid == 0) {

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

      // Print waiting
      printf("Waiting...\t\t");
      fflush(stdout);
      // Wait for interrupt to occur
      pause();

      // Register parent signal handler
      signal(SIGINT, sigHandlerParent);
    }
  }

  return 0;
}

void sigHandlerParent(int sigNum) {
  switch(sigNum) {
  case SIGINT:
    // Kill child
    // kill();

    // Print we're leaving
    printf ("outta here.\n");

    // Exit program
    exit(0);
    break;

  case SIGUSR1:
    printf("SIGUSR1 detected...\n");
    break;

  case SIGUSR2:
    printf("SIGUSR2 detected...\n");
    break;

  default:
    printf("Unknown signal detected...\n");
    break;
  }
}

// Child signal handler
void sigHandlerChild(int sigNum) {
  // On receiving a signal, exit out of process
  exit(0);
}
