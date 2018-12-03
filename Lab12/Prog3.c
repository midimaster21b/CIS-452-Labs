/***************************************************************
 * Title: Prog3.c
 * Authors: Joshua Edgcombe <joshedgcombe@gmail.com>
 *          Patton Finley <finleyp@mail.gvsu.edu>
 *
 * Description: A simple program to use the system calls to
 *              interface with the filesystem similar to ls.
 **************************************************************/
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
  DIR *dirPtr;
  struct dirent *entryPtr;
  struct stat statBuf;

  int n_flag = 0;
  int i_flag = 0;
  int x = 0;

  if(argc > 2) {
    for(x=1; x<3; x++) {
      // Process arguments
      if(strcmp(argv[x], "-i") == 0) {
        i_flag = 1;
        break;
      }
      else if(strcmp(argv[x], "-n") == 0) {
        n_flag = 1;
        break;
      }
      else if(strcmp(argv[x], "-in") == 0) {
        i_flag = 1;
        n_flag = 1;
        break;
      }
      else if(strcmp(argv[x], "-ni") == 0) {
        i_flag = 1;
        n_flag = 1;
        break;
      }
      else if(strcmp(argv[x], "i") == 0) {
        i_flag = 1;
        break;
      }
      else if(strcmp(argv[x], "n") == 0) {
        n_flag = 1;
        break;
      }
      else if(strcmp(argv[x], "in") == 0) {
        i_flag = 1;
        n_flag = 1;
        break;
      }
      else if(strcmp(argv[x], "ni") == 0) {
        i_flag = 1;
        n_flag = 1;
        break;
      }
    }
  }

  char *filename;

  if(x == 1) {
    filename = argv[2];
  }
  else {
    filename = argv[1];
  }

  // Stat the file or directory supplied
  stat(filename, &statBuf);

  // Check if file or dir exists
  if(statBuf.st_ino == 0) {
    printf("Error: File not found\n");
    return 1;
  }

  // If directory
  if((S_IFDIR & statBuf.st_mode) > 0) {
    // Determine if arguments were specified first or last
    if(x == 1) {
      dirPtr = opendir(argv[2]);
    }
    else {
      dirPtr = opendir(argv[1]);
    }

    while((entryPtr = readdir(dirPtr))) {
      // Fill the stat struct
      stat(entryPtr->d_name, &statBuf);

      if(i_flag) {
        printf("%ld\t", statBuf.st_ino);
      }

      if(n_flag) {
        printf("%d:%d\t", statBuf.st_uid, statBuf.st_gid);
      }

      printf("%-20s\n", entryPtr->d_name);
    }

    closedir(dirPtr);
  }

  // If regular file
  else {
    if(i_flag) {
      printf("%ld\t", statBuf.st_ino);
    }

    if(n_flag) {
      printf("%d:%d\t", statBuf.st_uid, statBuf.st_gid);
    }

    printf("%-20s\n", filename);
  }


  return 0;
}
