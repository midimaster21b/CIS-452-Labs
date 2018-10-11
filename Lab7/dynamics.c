#include <stdio.h>
#include <unistd.h>

int main(void) {
  printf("Dynamic System Resource Detection\n");
  printf("Page size (bytes): %ld\n", sysconf(_SC_PAGESIZE));
  printf("Number of physical pages: %ld\n", sysconf(_SC_PHYS_PAGES));


}
