/*****************************************************************
 * Title: comm_process_ec.c
 * Authors: Joshua Edgcombe <joshedgcombe@gmail.com>
 *          Patton Finley <finleyp@mail.gvsu.edu>
 *
 * Description: A program showing the use of the sigaction method
 *              for registering linux signal handler functions.
 *****************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

// Prototypes for the signal handlers
void sigHandlerParent(int, siginfo_t *, void *);
void sigHandlerChild(int, siginfo_t *, void *);

// Global variables for tracking parent and child PID's
pid_t child_pid[2], parent_pid;
struct sigaction *sig_act_struct;

int main() {
  // Process ID variables
  child_pid[0] = fork();

  // Random number
  srand(time(NULL));
  int r = rand();

  // 1st Child process
  if(child_pid[0] == 0) {
    // Re-seed child process
    srand(r);

    // Get parent process PID
    parent_pid = getppid();

    // Instantiate sigaction struct
    sig_act_struct = malloc(sizeof(struct sigaction));
    sig_act_struct->sa_sigaction = sigHandlerChild; // Register handler function
    sig_act_struct->sa_flags = SA_SIGINFO; // Use 3 arg handler (to get process info)
  }

  // Parent process
  else if(child_pid > 0) {
    // Create child 2
    child_pid[1] = fork();

    // 2nd Child process
    if(child_pid[1] == 0) {
      // Re-seed child process
      srand(rand());

      // Get parent process PID
      parent_pid = getppid();

      // Instantiate sigaction struct
      sig_act_struct = malloc(sizeof(struct sigaction));
      sig_act_struct->sa_sigaction = sigHandlerChild; // Register handler function
      sig_act_struct->sa_flags = SA_SIGINFO; // Use 3 arg handler (to get process info)
    }

    // Parent process continued
    else if(child_pid[1] > 0) {
      // Instantiate sigaction struct
      sig_act_struct = malloc(sizeof(struct sigaction));
      sig_act_struct->sa_sigaction = sigHandlerParent; // Register handler function
      sig_act_struct->sa_flags = SA_SIGINFO; // Use 3 arg handler (to get process info)


    }
  }

  // Error handling (child_pid < 0)
  else if(child_pid < 0){
    printf("Error: Fork failed.\n");
    return(1);
  }

  while(1) {
    // Child processes
    if(child_pid[0] == 0 || child_pid[1] == 0) {
      // Register child signal handler
      /* signal(SIGINT, sigHandlerChild); */
      if(sigaction(SIGINT, sig_act_struct, NULL)) {
	printf("ERROR: An error occurred while registering the signal handler for SIGINT in the child process.\n");
	free(sig_act_struct);
	return(1);
      }

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
    else {
      // Register signal handlers
      /* signal(SIGINT, sigHandlerParent); */
      /* signal(SIGUSR1, sigHandlerParent); */
      /* signal(SIGUSR2, sigHandlerParent); */
      if(sigaction(SIGINT, sig_act_struct, NULL)) {
	printf("ERROR: An error occurred while registering the signal handler for SIGINT in the parent process.\n");
	free(sig_act_struct);
	kill(child_pid[0], SIGINT);
	kill(child_pid[1], SIGINT);
	return(1);
      }

      if(sigaction(SIGUSR1, sig_act_struct, NULL)) {
	printf("ERROR: An error occurred while registering the signal handler for SIGUSR1 in the parent process.\n");
	free(sig_act_struct);
	kill(child_pid[0], SIGINT);
	kill(child_pid[1], SIGINT);
	return(1);
      }

      if(sigaction(SIGUSR2, sig_act_struct, NULL)) {
	printf("ERROR: An error occurred while registering the signal handler for SIGUSR2 in the parent process.\n");
	free(sig_act_struct);
	kill(child_pid[0], SIGINT);
	kill(child_pid[1], SIGINT);
	return(1);
      }

      // Print waiting and flush to the terminal (print immediately)
      printf("Waiting...\t\t");
      fflush(stdout);

      // Wait for interrupt to occur
      pause();
    }
  }

  return 0;
}

void sigHandlerParent(int sig_num, siginfo_t *sig_inf, void *context) {

  pid_t child_proc = sig_inf->si_pid;

  // Switch on the supplied signal
  switch(sig_num) {
  case SIGINT:
    // Kill the child
    kill(child_pid[0], SIGINT);
    kill(child_pid[1], SIGINT);

    // Print we're leaving
    printf(" received. That's it, I'm shutting down...\n");

    // Exit program
    exit(0);
    break;

  case SIGUSR1:
    // Print appropriate signal to the terminal
    printf("SIGUSR1 detected... (%d)\n", child_proc);
    break;

  case SIGUSR2:
    // Print appropriate signal to the terminal
    printf("SIGUSR2 detected... (%d)\n", child_proc);
    break;

  default:
    // Handle unknown signals
    printf("Unknown signal detected...\n");
    break;
  }
}

// Child signal handler
void sigHandlerChild(int sig_num, siginfo_t *sig_inf, void *context) {
  // On receiving a signal, exit out of process
  free(sig_act_struct);

  exit(0);
}
