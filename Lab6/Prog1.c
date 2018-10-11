#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "mysem.h"

#define SIZE 16

int main(int argc, char **argv) {
  int status;
  long int i, loop, temp, *shmPtr;
  int shmId;
  pid_t pid;

  // Semaphore variables
  int sem_id;

  // Create a private semaphore
  sem_id = sem_create();

  // Initialize semaphore value to 1
  sem_init(sem_id);

  // get value of loop variable (from command-line argument)
  if(argc > 1) {
    loop = strtol(*(argv+1), NULL, 10);
    printf("Number of swaps: %ld\n", loop);
  }
  else {
    perror("Usage: Prog1 <num_swaps>\n");
    exit(1);
  }

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

  printf("shmPtr[0]: %p\n", &shmPtr[0]);
  printf("shmPtr[1]: %p\n", &shmPtr[1]);

  // Child behavior
  if(!(pid = fork())) {
    for(i=0; i<loop; i++) {
      // Wait for semaphore
      sem_wait(sem_id);

      // swap the contents of shmPtr[0] and shmPtr[1]
      temp = shmPtr[0];
      shmPtr[0] = shmPtr[1];
      shmPtr[1] = temp;

      // Signal (increment semaphore by 1)
      sem_signal(sem_id);
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
      // Wait (decrement semaphore by 1)
      sem_wait(sem_id);

      // swap the contents of shmPtr[1] and shmPtr[0]
      temp = shmPtr[1];
      shmPtr[1] = shmPtr[0];
      shmPtr[0] = temp;

      // Signal (increment semaphore by 1)
      sem_signal(sem_id);
    }
  }

  // Wait for child process to finish
  wait(&status);

  // Print values of pointer locations
  printf("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

  // Remove semaphore
  sem_destroy(sem_id);

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
