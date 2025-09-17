#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.
#include <assert.h> // assert.



int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: file path\n");
    return EXIT_FAILURE;
  }

  if (fopen(argv[1], "r") == NULL) {
    print_error(argv[1], errno);
    return EXIT_FAILURE;

  } else {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "file %s", argv[1]);
    system(cmd);
    return EXIT_SUCCESS;
  }
}

int print_error(char *path, int errnum){
  return fprintf(stdout, "%s: cannot determine (%s)\n", path, strerror(errnum));
}
