#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {

  if(argc < 3 || argc > 4) {
    printf("Usage: my_ln [options] [source_file] [destination_file]\n");
    return 1;
  }

  int soft_link_flag = 0;
  int src_index = 1;
  int dest_index = 2;

  // Find options [if present] and adjust src and dest index as necessary
  for(int i=0; i<argc; i++) {
    if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "s") == 0) {
      if(i == 1) {
        // Verify there are enough arguments
        if(argc < 4) {
          printf("Error: invalid number of parameters supplied.");
        }

        // Update source and destination index
        src_index = 2;
        dest_index = 3;
      }
      else if(i == 2) {
        // Verify there are enough arguments
        if(argc < 4) {
          printf("Error: invalid number of parameters supplied.");
        }

        // Update source and destination index
        src_index = 1;
        dest_index = 3;
      }
      else if(i == 3) {
        // Verify there are enough arguments
        if(argc < 4) {
          printf("Error: invalid number of parameters supplied.");
        }

        // Update source and destination index
        src_index = 1;
        dest_index = 2;
      }

      soft_link_flag = 1;
    }
  }

  if(soft_link_flag) {
    // Create a soft link
    if(symlink(argv[src_index], argv[dest_index]) == -1) {
      perror("Error occurred: ");
      return 1;
    }
  }

  else {
    // Create a hard link
    if(link(argv[src_index], argv[dest_index]) == -1) {
      perror("Error occurred: ");
      return 1;
    }
  }

  return 0;
}
