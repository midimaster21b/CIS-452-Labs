#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <ulimit.h>

int main(void) {
  struct rlimit temp;
  /* long file_size; */

  printf("Dynamic System Resource Detection\n");
  printf("Page size (bytes): %ld\n", sysconf(_SC_PAGESIZE));
  printf("Number of physical pages: %ld\n", sysconf(_SC_PHYS_PAGES));
  printf("Max number of processes per user: %ld\n", sysconf(_SC_CHILD_MAX));

  // File size limit
  if(getrlimit(RLIMIT_FSIZE, &temp) == 0) {
    if(temp.rlim_cur == RLIM_INFINITY) {
      printf("Max file size (soft bytes): No OS-based Limit\n"); // RLIMIT_FSIZE
    }
    else {
      printf("Max file size (soft bytes): %ld\n", temp.rlim_cur); // RLIMIT_FSIZE
    }

    if(temp.rlim_max == RLIM_INFINITY) {
      printf("Max file size (hard bytes): No OS-based Limit\n"); // RLIMIT_FSIZE
    }
    else {
      printf("Max file size (hard bytes): %ld\n", temp.rlim_max); // RLIMIT_FSIZE
    }
  }

  /* if(ulimit(UL_GETFSIZE, &file_size) != -1) { */
  /*   printf("Max file size (bytes): %ld\n", file_size * 512); // UL_GETFSIZE */
  /* } */
  else {
    // An error occurred.
    printf("An error occurred while retrieving the max file size limit.\n");
  }

  // Max # of files
  if(getrlimit(RLIMIT_NOFILE, &temp) == 0) {
    printf("Max # of open files (Soft limit): %ld\n", temp.rlim_cur); // RLIMIT_NOFILE
    printf("Max # of open files (Hard limit): %ld\n", temp.rlim_max); // RLIMIT_NOFILE
  }
  else {
    // An error occurred.
    printf("An error occurred while retrieving the max # of open files limit.\n");
  }

  printf("Clock ticks per second: %ld\n", sysconf(_SC_CLK_TCK)); // _SC_CLK_TCK

}
