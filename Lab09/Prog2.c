#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int t;
const int r = 0;
const int v = 0;

int main() {
  int x;
  int y = 1;

  char *z = malloc(1);
  char *w = malloc(1);

  printf("Uninitialized global integer: %p\n", &t);
  printf("Constant integer 1: %p\n", &r);
  printf("Constant integer 2: %p\n", &v);
  printf("Uninitialized variables: %p\n", &x);
  printf("Initialized variables: %p\n", &y);
  printf("Malloc'd variable 1: %p\n", z);
  printf("Malloc'd variable 2: %p\n", w);
  printf("Shared C library (printf): %p\n", &printf);

  pause();

  return 0;
}
