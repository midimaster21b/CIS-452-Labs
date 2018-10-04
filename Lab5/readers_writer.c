/***************************************************************
 * Title: readers_writer.c
 * Authors: Joshua Edgcombe <joshedgcombe@gmail.com>
 *          Patton Finley <finleyp@mail.gvsu.edu>
 *
 * Description: A simple program showing the appropriate use of
 *              linux shared memory for a basic chat app.
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct msg_mem_header {
  // Number of attached clients
  int count;
  pthread_mutex_t count_lock;

  // Write protection
  pthread_mutex_t write_lock;
  pthread_mutex_t read_lock;
} msg_mem_header;

#define MAX_MESSAGE_LENGTH 1024
#define MESSAGE_OFFSET sizeof(msg_mem_header)
#define MEM_SIZE (MAX_MESSAGE_LENGTH + MESSAGE_OFFSET)

void signal_handler(int SIGNUM);
void *writer_thread(void *shared_mem_region);
void *reader_thread(void *shared_mem_region);

const char *key_path = ".";
const int project_id = 1;

int shmId;
char *shmPtr;

int main(void) {

  int first_time_flag = 0;
  int status;
  msg_mem_header *header;
  pthread_t temp_thread_handle;

  // Register signal handler function
  signal(SIGINT, signal_handler);

  printf("Welcome to the reader program!\n");
  printf("==============================\n");

  int shared_mem_key = ftok(key_path, project_id);

  printf("Retrieving memory region...\n");

  // Retrieve the shared memory region
  if((shmId = shmget(shared_mem_key, shared_mem_key, S_IRUSR|S_IWUSR)) < 0){
    printf("Shared memory region not found, creating now...\n");

    // Allocate shared memory
    if((shmId = shmget(shared_mem_key, shared_mem_key, IPC_CREAT|S_IRUSR|S_IWUSR)) >= 0) {
      // Set first time flag for initialization
      first_time_flag = 1;
    }

    else {
      // Handle error during initialization
      perror("ERROR: Unable to retrieve a shared memory segment.\n");
      exit(1);
    }
  }

  printf("Attaching memory region to client...\n");

  // Attach to shared memory
  if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
    perror("ERROR: Unable to attach to shared memory segment.\n");
    exit(1);
  }

  printf("Memory region attached...\n");

  // If first client
  if(first_time_flag) {
    // Perform first client initialization
    // Set header to location of shared memory
    header = (msg_mem_header *)shmPtr;

    // Set count to 1
    header->count = 1;

    // Initialize count mutex
    if((status = pthread_mutex_init(&(header->count_lock), NULL)) != 0) {
      printf("Mutex init error %d: %s\n", status, strerror(status));

      // Not so elegant exit routine
      signal_handler(SIGINT);
    }

    // Initialize write mutex
    if((status = pthread_mutex_init(&(header->write_lock), NULL)) != 0) {
      printf("Mutex init error %d: %s\n", status, strerror(status));

      // Not so elegant exit routine
      signal_handler(SIGINT);
    }

    // Initialize read mutex
    if((status = pthread_mutex_init(&(header->read_lock), NULL)) != 0) {
      printf("Mutex init error %d: %s\n", status, strerror(status));

      // Not so elegant exit routine
      signal_handler(SIGINT);
    }
  }

  // Not first time attach
  else {
    // Set header to location of shared memory
    header = (msg_mem_header *)shmPtr;

    // Increment number of clients
    pthread_mutex_lock(&(header->count_lock));
    header->count++;
    pthread_mutex_unlock(&(header->count_lock));
  }

  // Create threads
  pthread_create(&temp_thread_handle, NULL, writer_thread, shmPtr);
  pthread_create(&temp_thread_handle, NULL, reader_thread, shmPtr);

  // Busy wait until interrupt
  while(1);

  return 0;
}

void signal_handler(int SIGNUM) {
  int count;

  printf("Exiting...\n");

  // Set header to location beginning of shared memory
  msg_mem_header *header = (msg_mem_header *)shmPtr;

  // Decrement count and store into count
  pthread_mutex_lock(&(header->count_lock));
  count = --header->count;
  pthread_mutex_unlock(&(header->count_lock));

  // Detach from shared memory
  if(shmdt(shmPtr) < 0) {
    perror("ERROR: Unable to detach from shared memory.\n");
    exit(1);
  }

  // If last client using shared memory region
  if(count <= 1) {
    printf("Freeing shared memory region...\n");

    // Deallocate shared memory
    if(shmctl(shmId, IPC_RMID, 0) < 0) {
      perror ("ERROR: Unable to deallocate shared memory.\n");
      exit(1);
    }
  }

  exit(0);
}

void *writer_thread(void *shared_mem_region) {
  // Detach the worker thread from the dispatcher
  pthread_detach(pthread_self());

  char writer_contents[MAX_MESSAGE_LENGTH] = {0};

  // Message pointer
  char *shmMsgPtr = (shared_mem_region+MESSAGE_OFFSET);

  // Setup message header memory region
  msg_mem_header *header = (msg_mem_header *)shmPtr;

  while(1) {
    printf("Please enter a string to write to shared memory: ");
    fgets(writer_contents, MAX_MESSAGE_LENGTH, stdin);

    // Update the contents of the current string
    pthread_mutex_lock(&(header->write_lock));

    // Copy string to shared memory region
    strcpy(shmMsgPtr, writer_contents);

    // Update the contents of the current string
    pthread_mutex_unlock(&(header->write_lock));
  }
}

void *reader_thread(void *shared_mem_region) {
  // Detach the worker thread from the dispatcher
  pthread_detach(pthread_self());

  char reader_contents[MAX_MESSAGE_LENGTH] = {0};

  // Message pointer
  char *shmMsgPtr = (shared_mem_region+MESSAGE_OFFSET);

  // Setup message header memory region
  msg_mem_header *header = (msg_mem_header *)shmPtr;

  while(1) {


    // If contents have been updated
    if(strcmp(shmMsgPtr, reader_contents) != 0) {

      // Forces only one reader at a time and gives priority to the writer.
      // Prevents printing of incomplete written messages as well.
      pthread_mutex_lock(&(header->read_lock));
      pthread_mutex_lock(&(header->write_lock));

      // Update the contents of the current string
      strcpy(reader_contents, shmMsgPtr);

      pthread_mutex_unlock(&(header->write_lock));
      pthread_mutex_unlock(&(header->read_lock));

      // Print out the updated string
      printf("\n%s", reader_contents);
    }
  }
}
