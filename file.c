#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.
#include <assert.h> // assert.



int main(int argc, char *argv[]) {
  assert (argc == 2);

  if (fopen(argv[1], "r") == NULL) {
    fprintf(stderr, "Error opening file '%s': %s\n", argv[1], strerror(errno));
    return EXIT_FAILURE;

  } else {
    fprintf(stdout, "File '%s' Exists.\n", argv[1]);
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "file %s", argv[1]);
    system(cmd);
    return EXIT_SUCCESS;
  }


}
