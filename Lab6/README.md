# CIS 452 Lab Week 6: Shared Memory Synchronization

## Overview
The purpose of this assignment is to become familiar with the methods used for synchronizing process access to shared memory; specifically, the coordination of process' entry into their critical sections.  The UNIX IPC package, in the form of shared memory and semaphores, will be investigated and used as the mechanism for controlling process synchronization.

## Activities
- Work your way through the following exercises, demonstrating your knowledge of the material by answering the numbered questions.  The lab and related programming exercise are intended to be performed with a partner.
- Submit a detailed lab report (one per group).  Include:
  + the answers to the numbered questions
  + your final source code
  + output from a sample run

## The Shared Memory Problem
Because of its high speed and low overhead, developers often use shared memory as an interprocess communication mechanism.  This technique is implemented by allowing processes to share the same data space.  However, asynchronous access to shared memory may result in unusual and incorrect results.  As mentioned in class and in your textbook, shared memory access presents users with an instance of the Critical Section problem.  Recall the main issues involved with process synchronization while studying the following program:
Sample Program 1

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SIZE 16

int main ()
{
   int status;
   long int i, loop, temp, *shmPtr;
   int shmId;
   pid_t pid;

      // get value of loop variable (from command-line argument)

   if ((shmId = shmget (IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror ("i can't get no..\n");
      exit (1);
   }
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      perror ("can't attach\n");
      exit (1);
   }

   shmPtr[0] = 0;
   shmPtr[1] = 1;

   if (!(pid = fork())) {
      for (i=0; i<loop; i++) {
	       // swap the contents of shmPtr[0] and shmPtr[1]
      }
      if (shmdt (shmPtr) < 0) {
	 perror ("just can't let go\n");
	 exit (1);
      }
      exit(0);
   }
   else
      for (i=0; i<loop; i++) {
	       // swap the contents of shmPtr[1] and shmPtr[0]
      }

   wait (&status);
   printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

   if (shmdt (shmPtr) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }
   if (shmctl (shmId, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }

   return 0;
}
```

## Perform the following operations and answer the questions:

- study the Sample Program
- add the necessary code to implement the three comments (in bold)
  + you will need to adjust the function heading for **main()** and insert code to grab the argument to be stored into the variable **loop**
  + the **temp** variable is provided for your "swap" code

1. what exactly does Sample Program 1 intend to do (i.e. who is responsible for what operations)?
1. given that, what is the program's expected output?

    + compile Sample Program 1
    + run the program multiple times with increasing **loop** values (e.g. 10, 100, 1000, ...) until you observe interesting and unexpected results.

1. describe the output of the Sample Program as the **loop** values increase
1. describe precisely what is happening to produce the observed interesting output.  Your answer should tie in to the concepts discussed in Chapter 5 of your textbook -- Process Synchronization.

## Shared Memory Synchronization
This lab concerns itself primarily with mechanisms for controlling access to shared memory; specifically, it introduces semaphores.  Like shared memory, semaphores are a kernel resource and follow the familiar resource usage pattern: request, use, release.

For semaphores, the resource usage paradigm is expressed as follows:

1.  obtain a semaphore set (often of size 1)
1.  initialize the semaphore element(s)
1.  use the semaphores correctly (i.e. wait()...signal())
1.  remove the kernel resource data structure

Each of the steps above has an associated system call.  The system calls are very similar to the calls used for shared memory.  One of the main differences is that the functions are constructed to work with "sets" of semaphores - however, they can easily be used to manipulate a single semaphore.  The functions corresponding to the above steps are:

1. **semget()** - this function creates a semaphore set and initializes its elements to 0.  It initializes a kernel data structure for operating system management of the resource and returns a resource ID to the user.  This ID is used by any process wishing to use the semaphore.
1. **semctl()** - used for controlling the resource.  This function is typically used to set (i.e. initialize) or query the values of semaphore elements.
1. **semop()** - used for incrementing, decrementing and testing the value of semaphore elements.
1. **semctl()** - used for controlling the resource.  This function  is also used to "free" the resource; removing the semaphore and its associated data structures from the system.

Refer to the man pages for additional details on semaphore operation (you may also find the include files bits/ipc.h , sys/sem.h and bits/sem.h useful).

The code snippets below demonstrate the use of these system calls:

```C
/*  create a new semaphore set for use by this (and other) processes..
*/
semId = semget (IPC_PRIVATE, 1, 00600));

/*  initialize the semaphore set referenced by the previously obtained semId handle.
*/
semctl (semId, 0, SETVAL, 1);

/*  the semop() function is used to perform operations on semaphore elements (for example,
 *  to do the equivalent of the Wait() and Signal() operations).  semId is the semaphore set
 *  ID obtained previously, sbuf is the name of a user variable (i.e. a data object) of type
 *  sembuf.  The values of the sembuf structure are filled in by the user and used by the system
 *  to determine the nature of the operation to be performed.  Refer to the man pages and the .h
 *  files for more detail on this structure and its use.
*/
semop (semId, &sbuf, 1);

//  remove the semaphore referenced by semId
semctl (semId, 0, IPC_RMID);
```

Note that the **semctl()** function call is used for initializing and also for removing the semaphore.  It uses a different syntax (and a different number of arguments) depending on the usage desired.

### Perform the following operations:

5. name and describe in your own words the use of the three fields of the sembuf structure
6. what is the purpose of the **SEM_UNDO** flag (i.e. why would you use it)?
  - Note: make sure you truly understand what this flag does.

## Miscellaneous Notes
- Recall that the system utility "ipcs" (IPC status) displays information about currently allocated kernel IPC resources, including semaphores.  The system utility "ipcrm" (IPC remove) is used for semaphore removal.  Use this utility to clean up resources in the event of programming errors.
- The process that creates shared memory or semaphores should be responsible for:
  + creation
  + verifying that no segment or semaphore set with the same ID number exists
  + establishing access permissions
  + cleanup and deallocation
- All of these tasks are controlled by the flags passed to the respective get() and ctl() functions.  As usual, read the man pages!
- Similar to signals, there are both System V and POSIX versions of semaphores.  All systems implement the SysV version (which you should use for this lab).  Some systems also include POSIX semaphores, which are sometimes reserved for real-time threads.

## Programming Assignment (Controlled Process Synchronization)

The programmer's goal is to implement controlled, asynchronous access to shared memory; in this lab it is to properly synchronize access to the critical sections of Sample Program 1.  The main idea of this assignment is to demonstrate that with the use of proper synchronization mechanisms, the expected value is always obtained from the program.

- Protect the critical sections in Sample Program 1 to prevent memory access conflicts from causing inconsistencies in the output.  Your solution should still maximize potential parallelism.
  + insert the appropriate code to create and initialize a semaphore
  + use semaphore operations to synchronize the two processes
  + perform required cleanup operations
  + test that your Sample Program works correctly

**Note:** Semaphore creation and initialization are two different, hence non-atomic operations.  Be sure they have both been completed before another process attempts to access the semaphore.

## Extra Credit (Semaphore Toolkit)
Although their functionality is identical, these real-world semaphore operations look very different from the versions presented in class (and found in the textbook).  Can the differences be reconciled?

Create your own library consisting of a complete semaphore toolkit.  The idea is to be able to make simple function calls to your library that look similar to the semaphore operations described in class and in the textbook.  Your library should provide functionality to create, initialize, wait, signal, and destroy a semaphore.  For example:
- the prototype 'void Wait (int);'  declares the familiar 'wait(S)' function, and is found in an include file
- the actual code for the Wait() function would be defined and compiled into your library, and linked into your program
- the function would be used by a program as in 'Wait (semID);'
- This toolkit can then be re-used in the future whenever you are writing a program that requires process synchronization.

To create a library (toolkit):
- for a static library, begin by looking at **ar** and/or **ranlib**
- for a dynamicly-linked library, look for the appropriate compiler options to specify position-independent code and code-sharing
