#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.

int print_hello_world(void) {
  return fprintf(stdout, "Hello, world!\n");
}

int main(int argc, char *argv[]) {
  if (argc !=1) {
    fprintf("Only one argument is expected. \n");
    return EXIT_FAILURE;
  }

  if (print_hello_world() <= 0) {
    retval = EXIT_FAILURE;
  }
//testing now
  return retval;
}
