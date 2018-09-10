/******************************************************
 * Title: shell.c
 * Authors: Patton Finley <finleyp@mail.gvsu.edu>
 *          Joshua Edgcombe <joshedgcombe@gmail.com>
 *
 * Description: Simple shell program for lab 2 of
 *              CIS 452: Operating System Concepts
 ******************************************************/

#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>

void print_proc_stats();

int main() {

  return(0);
}

void print_proc_stats() {
  // Initialize variables for resource usage statistics
  int usage_retval;
  int process_who = RUSAGE_SELF;
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
