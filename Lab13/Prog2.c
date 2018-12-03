#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define SIZE 150

int main(int argc, char *argv[])
{
  struct flock fileLock;
  int fd;
  char buf[SIZE] = "";

  if(argc < 2) {
    printf("usage: filename\n");
    exit(1);
  }
  if((fd = open(argv[1], O_RDWR)) < 0) {
    perror("there is");
    exit(1);
  }

  fileLock.l_type = F_RDLCK;
  fileLock.l_whence = SEEK_SET;
  fileLock.l_start = 0;
  fileLock.l_len = 0;

  // Obtain the lock
  if(fcntl(fd, F_SETLKW, &fileLock) < 0) {
    perror("no way");
    exit(1);
  }

  // Read the first line
  read(fd, buf, SIZE);

  // Set flock struct to unlock
  fileLock.l_type = F_UNLCK;

  // Release the file lock
  if(fcntl(fd, F_SETLK, &fileLock) < 0) {
    perror("Error unlocking:");
    exit(1);
  }

  // Close the file
  close(fd);

  char temp_char;

  // Print out the first line
  for(int x=0; x<SIZE; x++) {
    temp_char = buf[x];

    if(temp_char == '\0' || temp_char == '\n') {
      break;
    }
    else {
      printf("%c", temp_char);
    }
  }

  printf("\n");

  return 0;
}
