/***************************************************************
 * Title: Prog2.c
 * Authors: Joshua Edgcombe <joshedgcombe@gmail.com>
 *          Patton Finley <finleyp@mail.gvsu.edu>
 *
 * Description: A simple program that prints the contents of
 *              the current directory.
 **************************************************************/
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
  DIR *dirPtr;
  struct dirent *entryPtr;
  struct stat statBuf;

  dirPtr = opendir(".");

  while((entryPtr = readdir(dirPtr))) {
    printf("%-20s", entryPtr->d_name);

    // Fill the stat struct
    stat(entryPtr->d_name, &statBuf);

    printf(" \t %ld bytes\n", statBuf.st_size);
  }

  closedir(dirPtr);

  return 0;
}
