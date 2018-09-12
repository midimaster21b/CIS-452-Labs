/******************************************************
 * Title: shell.c
 * Authors: Joshua Edgcombe <joshedgcombe@gmail.com>
 *          Patton Finley <finleyp@mail.gvsu.edu>
 *
 * Description: Simple shell program for lab 2 of
 *              CIS 452: Operating System Concepts
 ******************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

#include <ctype.h>

#define INPUT_BUFFER_SIZE 256

void print_proc_stats(int proc_who);
char *trim_whitespace(char *str);

int main(int argc, char*argv[]) {

  // Initialize variables
  char buffer[INPUT_BUFFER_SIZE];
  char* vector[INPUT_BUFFER_SIZE];
  int spot;
  int child_status;

  // Prompt the user to enter a command
  puts("This is a shell program for UNIX systems. Please insert UNIX commands: ");

  // Main program loop (REPL)
  while(1) {

    // Get the user input string
    fgets(buffer, INPUT_BUFFER_SIZE - 1, stdin);

    // Tokenize the user input string
    for(spot = 0; buffer[spot]!='\0'; spot++) {
      vector[spot] = strtok(buffer, " ");
    }

    // Check if the user input the "quit" command
    if(!strcmp(vector[0], "quit")){
      puts("Exiting");
      break;
    }

    // Create a new process for running the new command
    pid_t child_pid = fork();

    // Fork error occurred
    if(child_pid < 0) {

      // Print the error to the user
      perror("Error: An error occurred when forking the command.\n");

      // Return from the program
      return(1);
    }

    // Parent code
    else if(child_pid) {

      // Wait for the child process to complete
      waitpid(-1, &child_status, WUNTRACED);

      /* proc_pid = wait(&proc_retval); */
      /* print_proc_stats(RUSAGE_CHILDREN); */
    }

    // Child code
    else {
      printf("Child: %s\n", vector[0]);
      execvp(vector[0], (char *const *)NULL);
      break;
      // Exec the appropriate file
      /* execvp(vector[0],vector); */
      /* execv("ls", NULL); */
    }
  }

  return(0);
}

void print_proc_stats(int proc_who) {
  // Initialize variables for resource usage statistics
  int usage_retval;
  int process_who = proc_who;
  struct rusage process_usage;
  time_t process_user_seconds;
  long   process_user_microseconds;
  long   process_inv_cont_switches;

  // Get process resource usage
  usage_retval = getrusage(process_who, &process_usage);

  // Check return values
  if(usage_retval < 0) {
    printf("Error: An error occurred while getting usage information.\n");
  } else if(usage_retval > 0) {
    printf("Error: Unexpected positive nonzero return value received for 'getrusage()'\n");
  }

  // Get necessary usage statistic values
  process_user_seconds = process_usage.ru_utime.tv_sec;
  process_user_microseconds = process_usage.ru_utime.tv_usec;
  process_inv_cont_switches = process_usage.ru_nivcsw;

  // Output usage statistics information
  printf("Usage information\n");
  printf("=================\n");
  printf("User CPU time used: %02ld s, %ld us\n", process_user_seconds, process_user_microseconds);
  printf("Involuntary context switches: %ld\n", process_inv_cont_switches);
}

char *trim_whitespace(char *str) {
  // Remove leading and trailing whitespace
  char *temp = str;
  char *temp_two;

  // Remove leading whitespace
  while(isspace(*temp))
    temp++;

  // Move past all non-space characters
  temp_two = temp;
  while(!isspace(*temp_two))
    temp_two++;

  // End string (Removing all following whitespace)
  *temp_two = '\0';

  return temp;
}
