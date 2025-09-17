#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.

int print_hello_world(void) {
  return fprintf(stdout, "Hello, world!\n");
}

int main(int argc, char *argv[]) {
  assert (argc == 2);

  if (fopen(argv[1], "r") == NULL) {
    fprintf(stderr, "Error opening file '%s': %s\n", argv[1], strerror(errno));
    return EXIT_FAILURE;
  }

  if (print_hello_world() <= 0) {
    retval = EXIT_FAILURE;
  }
//testing now
  return retval;
}
