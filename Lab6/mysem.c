#include <sys/sem.h>

#include "mysem.h"

// Create semaphore resource
int  sem_create(void) {
  return semget(IPC_PRIVATE, 1, 00600);
}

// Initialize semaphore
void sem_init(int sem_id) {
  // Initialize semaphore value to 1
  semctl(sem_id, 0, SETVAL, 1);
}

// Semaphore wait function
void sem_wait(int sem_id) {
  struct sembuf wait_sbuf;

  // Initialize wait sembuf
  wait_sbuf.sem_num = 0;  // Semaphore zero in set
  wait_sbuf.sem_op  = -1; // Decrement the semaphore
  wait_sbuf.sem_flg = 0;  // No flags

  // Decrement the semaphore value and wait for a postive value to return
  semop(sem_id, &wait_sbuf, 1);
}

// Semaphore signal function
void sem_signal(int sem_id) {
  struct sembuf signal_sbuf;

  // Initialize signal sembuf
  signal_sbuf.sem_num = 0;  // Semaphore zero in set
  signal_sbuf.sem_op  = 1; // Decrement the semaphore
  signal_sbuf.sem_flg = 0;  // No flags

  semop(sem_id, &signal_sbuf, 1);
}

// Destroy semaphore resource
void sem_destroy(int sem_id) {
  // Remove semaphore
  semctl(sem_id, 0, IPC_RMID);
}
