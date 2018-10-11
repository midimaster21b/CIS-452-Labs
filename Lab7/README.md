# CS 452 mini-Lab Week 7: System Resource Utilization

## Overview
As discussed in class,  mismanagement of resources is a potential source of deadlock.  Malfunctioning drivers and rogue processes can be responsible for a denial of service to other users.  The purpose of this assignment is to become familiar with several methods used to determine the limits placed on system resources.  Both static and dynamic mechanisms will be introduced.

## Activities
- Submit limit values for the resources listed in the table.
  + Note: include representative code used to determine these values.
- As usual, this is intended to be a group project

## System Resource Limitations
When working with user or system resources such as processes, files, pipes, shared memory and semaphores, it is prudent to be aware of any system limits placed upon ownership of these resources.  This lab introduces several different methods for discovering the values of system limits for specific machine/operating system combinations.

### Static
In this context, static means "not active" (i.e. just looking it up).  As usual, the first place to start is the man pages and associated include files (always a good idea for a developer).  General information about system resources can be found in *limits.h* and *sys/param.h* but it may not be machine specific. Be sure to examine *bits/posix1_lim.h* and *bits/posix2_lim.h* in the event your specific system configuration supports the Posix standard for that resource. Note that limits obtained statically may not be reflective of actual runtime values; the source code defaults may have been overridden during compilation or in the course of a local installation.

### Dynamic
Many user limits can be obtained dynamically via the **getrlimit()** system call and the **sysconf()** function.  System limits can be determined via the **sysinfo()** system call.  All of them work by querying the kernel for the actual value of a resource.  They pass a flag to the system that specifies the resource of interest; the appropriate value is then either returned directly or written into a structure for subsequent access.  Read the man pages for details on how to use these functions at run-time (i.e. from within an executing program).

### Empirical
Finally, some limits are not defined in any include file, or available via query.  In this case, it is still possible to determine a maximum value experimentally.  Recall that all system calls return error codes indicating their success/failure (and that you should be checking those values!).  Obviously, a carefully-designed program could "push the limit" and use the return value to determine the point at which a system call fails, and hence the limit of the resource it is requesting.  For example, the **shmget()** function takes a parameter specifying the size of the shared memory segment desired.  It returns a segment ID if successful, or -1 on failure.  The size argument can be manipulated within your test program to determine the maximum-sized shared memory segment allowed on your system.


## Problem Specification
- use the specified method to determine the values for the following system resources.  Indicate what machine you performed your tests on and provide details regarding your method (i.e. name of include file, system call used, etc.)

 System Object	| Method | Value | Details
 Maximum # of semaphores per process | static | |
 Maximum value of a (counting) semaphore | static | |
 Maximum value of a (counting) semaphore | empirical | |
 Maximum size of a shared memory segment (bytes) | empirical | |
 Page size (bytes) | dynamic | |
 Physical pages in system | dynamic | |
 Maximum # of processes per user | dynamic | |
 Maximum filesize (bytes) | dynamic | |
 Maximum # of open files: hard limit | dynamic | |
 Maximum # of open files: soft limit | dynamic | |
 Clock resolution (msec) | dynamic | |
