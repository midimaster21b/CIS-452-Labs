#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SIZE 16

int main()
{
  int status;
  long int i, loop, temp, *shmPtr;
  int shmId;
  pid_t pid;

  // get value of loop variable (from command-line argument)

  // Get shared memory region.
  if((shmId = shmget(IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
    perror("Error: Failed to retrieve private shared memory region identifier.\n");
    exit(1);
  }
  if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
    perror("Error: Failed to attach shared memory region to process.\n");
    exit(1);
  }

  shmPtr[0] = 0;
  shmPtr[1] = 1;

  // Child behavior
  if(!(pid = fork())) {
    for(i=0; i<loop; i++) {
      // swap the contents of shmPtr[0] and shmPtr[1]
    }

    // Detach shared memory region
    if(shmdt(shmPtr) < 0) {
      perror("Error: Child process failed to detach shared memory region.\n");
      exit(1);
    }

    // Exit process
    exit(0);
  }

  // Parent behavior
  else {
    for (i=0; i<loop; i++) {
      // swap the contents of shmPtr[1] and shmPtr[0]
    }
  }

  // Wait for child process to finish
  wait(&status);

  // Print values of pointer locations
  printf("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

  // Detach shared memory region
  if(shmdt(shmPtr) < 0) {
    perror("just can't let go\n");
    exit(1);
  }

  // Destroy/deallocate shared memory region
  if(shmctl(shmId, IPC_RMID, 0) < 0) {
    perror("can't deallocate\n");
    exit(1);
  }

  return 0;
}
