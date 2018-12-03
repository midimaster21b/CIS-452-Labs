/***************************************************************
 * Title: Prog1.c
 * Authors: Joshua Edgcombe <joshedgcombe@gmail.com>
 *          Patton Finley <finleyp@mail.gvsu.edu>
 *
 * Description: A program that prints whether a file is a
 *              regular file or a directory
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
  struct stat statBuf;

  if(argc < 2) {
    printf("Usage: filename required\n");
    exit(1);
  }

  if(stat(argv[1], &statBuf) < 0) {
    perror("huh?  there is ");
    exit(1);
  }

  if((S_IFREG & statBuf.st_mode) > 0) {
    printf("Regular file\n");
  }
  else if((S_IFDIR & statBuf.st_mode) > 0) {
    printf("Directory\n");
  }

  /* printf("value is: %u\n", statBuf.st_mode);  */
  return 0;
}
