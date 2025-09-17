#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.
#include <assert.h> // assert.

//2.1 vi mangler på side 3 non-empty substring ikke lavet

enum file_type {
    DATA,
    EMPTY,
    ASCII,
    ISO_8859,
    UTF8,
};

const char * const FILE_TYPE_STRINGS[] = {
    "data",
    "empty",
    "ASCII text",
    "ISO-8859 text",
    "UTF-8 Unicode text"
};

enum file_type identify_file_type(const unsigned char *bytes, size_t length) {
    if (length == 0) 
      return EMPTY;
    return DATA;
}

int print_error(char *path, int errnum){
  return fprintf(stdout, "%s: cannot determine (%s)\n", path, strerror(errnum));
}


int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: file path\n");
    return EXIT_FAILURE;
  }

  if (fopen(argv[1], "r") == NULL) {
    print_error(argv[1], errno);
    return EXIT_SUCCESS;

  } else {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "file %s", argv[1]);
    system(cmd);
    return EXIT_SUCCESS;
  }
}

