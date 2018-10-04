/***************************************************************
 * Title: readers_writer.c
 * Authors: Joshua Edgcombe <joshedgcombe@gmail.com>
 *          Patton Finley <finleyp@mail.gvsu.edu>
 *
 * Description: A simple program showing the appropriate use of
 *              linux shared memory.
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <string.h>

#define MAX_MESSAGE_LENGTH 1024
#define MESSAGE_OFFSET 0
#define MEM_SIZE (MAX_MESSAGE_LENGTH + MESSAGE_OFFSET)
#define SHM_KEY 1024

void signal_handler(int SIGNUM);

const char *key_path = "/home/midimaster21b/School/CIS452/Labs/Lab5/readers_writer.c";
const int project_id = 1;

int shmId;
char *shmPtr;
char contents[MAX_MESSAGE_LENGTH] = {0};

int main(void) {

  // Register signal handler function
  signal(SIGINT, signal_handler);

  printf("Welcome to the reader program!\n");
  printf("==============================\n");

  char *shmMsgPtr;
  int shared_mem_key = ftok(key_path, project_id);

#ifdef WRITER
  // Allocate shared memory
  if((shmId = shmget(shared_mem_key, shared_mem_key, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0)
#else
  // Retrieve the shared memory region
  if((shmId = shmget(shared_mem_key, shared_mem_key, S_IRUSR|S_IWUSR)) < 0)
#endif
  {
    perror("ERROR: Unable to retrieve a shared memory segment.\n");
    exit(1);
  }

  // Attach to shared memory
  if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
    perror("ERROR: Unable to attach to shared memory segment.\n");
    exit(1);
  }

  shmMsgPtr = (shmPtr+MESSAGE_OFFSET); // Message pointer

  printf("Shared memory pointer: %p\n", shmPtr);
  printf("Shared memory msg pointer: %p\n", shmMsgPtr);
  printf("Shared memory id: %d\n", shmId);
  printf("Shared memory generated key: %d\n", shared_mem_key);

  printf("Current data: %s", shmMsgPtr);

  // Write to/read from memory
  while(1) {
#ifdef WRITER
    printf("Please enter a string to write to shared memory: ");
    fgets(contents, MAX_MESSAGE_LENGTH, stdin);

    // Update the contents of the current string
    strcpy(shmMsgPtr, contents);

    printf("\nData (%p): %s", shmMsgPtr, shmMsgPtr);
#else
    // If contents have been updated
    if(strcmp(shmMsgPtr, contents) != 0) {
      printf("New message found...\n");

      // Update the contents of the current string
      strcpy(contents, shmMsgPtr);

      // Print out the updated string
      printf("%s", contents);
    }
    else {

    }
#endif
  }

  // Detach from shared memory
  /* if(shmdt(shmPtr) < 0) {  */
  /*   perror("ERROR: Unable to detach from shared memory.\n");  */
  /*   exit(1);  */
  /* }  */

  // Deallocate shared memory
  /* if (shmctl (shmId, IPC_RMID, 0) < 0) {  */
  /*   perror ("ERROR: Unable to deallocate shared memory.\n");  */
  /*   exit(1);  */
  /* } */

  return 0;
}

void signal_handler(int SIGNUM) {
  printf("Exiting...\n");

  // Detach from shared memory
  if(shmdt(shmPtr) < 0) {
    perror("ERROR: Unable to detach from shared memory.\n");
    exit(1);
  }

#ifdef WRITER
  // Deallocate shared memory
  if (shmctl (shmId, IPC_RMID, 0) < 0) {
    perror ("ERROR: Unable to deallocate shared memory.\n");
    exit(1);
  }
#endif

  exit(0);
}
