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

#define SIZE 256

void print_proc_stats(int proc_who);

int main(int argc, char*argv[]) {

  puts("This is a shell program for UNIX systems. Please insert UNIX commands: ");

  char buffer[SIZE];
  char* vector[SIZE];
  int spot;
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

    pid_t child_pid = fork();

    // Fork error occurred
    if(child_pid < 0){
      perror("Error: An error occurred when forking the command.\n");
      exit(0);
    }

    // Parent code
    else if(child_pid){
      waitpid(-1, &status, WUNTRACED);

      /* proc_pid = wait(&proc_retval); */
      /* print_proc_stats(RUSAGE_CHILDREN); */
    }

    // Child code
    else{
      execvp(vector[0],vector);
      exit(0);

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
