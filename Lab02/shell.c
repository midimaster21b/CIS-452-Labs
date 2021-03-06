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
  int spot, x, child_status;
  char *temp;

  // Welcome the user to the shell
  puts("Welcome to the shell!\n");

  // Main program loop (REPL)
  while(1) {

    // Prompt the user to enter a command
    printf("Enter command: ");

    // Get the user input string
    fgets(buffer, INPUT_BUFFER_SIZE - 1, stdin);

    // Trim the whitespace of the buffer for comparison
    temp = trim_whitespace(buffer);

    // Check if the user input the "quit" command
    if(!strcmp(temp, "quit")){
      puts("Goodbye!");
      free(temp);
      break;
    }
    free(temp);

    // Tokenize the user input string
    spot = 0;

    while(1) {
      if(spot == 0) {
	vector[spot] = strtok(buffer, " ");
      } else {
	vector[spot] = strtok(NULL, " ");
      }

      if(vector[spot] == NULL) {
	break;
      }
      else {
	vector[spot] = trim_whitespace(vector[spot]);
	spot += 1;
      }
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
      // Clean up allocated memory
      for(x=0; x<=spot; x++) {
	free(vector[x]);
      }

      // Wait for the child process to complete
      waitpid(-1, &child_status, WUNTRACED);

      // Separate program output from statistics
      printf("\n");

      // Print the process usage statistics
      print_proc_stats(RUSAGE_CHILDREN);

      // Separate statistics from next prompt
      printf("\n");

    }

    // Child code
    else {
      // Exec the process
      execvp(vector[0], vector);
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

  static long process_cumulative_seconds = 0;
  static long process_cumulative_micros = 0;
  static long process_cumulative_inv_cont_switches = 0;

  // Get process resource usage
  usage_retval = getrusage(process_who, &process_usage);

  // Check return values
  if(usage_retval < 0) {
    printf("Error: An error occurred while getting usage information.\n");
  } else if(usage_retval > 0) {
    printf("Error: Unexpected positive nonzero return value received for 'getrusage()'\n");
  }

  // Get necessary usage statistic values
  process_user_seconds = process_usage.ru_utime.tv_sec - process_cumulative_seconds;
  process_user_microseconds = process_usage.ru_utime.tv_usec - process_cumulative_micros;
  process_inv_cont_switches = process_usage.ru_nivcsw - process_cumulative_inv_cont_switches;


  // Update cumulative stats
  process_cumulative_seconds = process_usage.ru_utime.tv_sec;
  process_cumulative_micros = process_usage.ru_utime.tv_usec;
  process_cumulative_inv_cont_switches = process_usage.ru_nivcsw;

  // Output usage statistics information
  printf("Usage information\n");
  printf("=================\n");
  printf("User CPU time used: %02ld s, %ld us\n", process_user_seconds, process_user_microseconds);
  printf("Involuntary context switches: %ld\n", process_inv_cont_switches);
}

char *trim_whitespace(char *str) {
  // Remove leading and trailing whitespace
  char *str_start = str;
  char *str_end;
  int str_size;

  // Remove leading whitespace
  while(isspace(*str_start))
    str_start++;

  // Move past all non-space characters
  str_end = str_start;
  while(!isspace(*str_end) && *str_end != '\0')
    str_end++;

  // Calculate necessary string size
  str_size = str_end - str_start;

  // Malloc space for retval (+1 for null character)
  char *retval = malloc(str_size + 1);

  // Copy string to return value
  retval = strncpy(retval, str_start, str_size);

  // Ensure string is null-terminated
  *(retval + str_size) = '\0';

  // Return the trimmed string
  return retval;
}
