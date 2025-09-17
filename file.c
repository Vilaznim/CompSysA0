#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.

// 2.1 vi mangler på side 3 non-empty substring ikke lavet

enum file_type
{
  DATA,
  EMPTY,
  ASCII,
  ISO_8859,
  UTF8,
  VERY_SHORT
};

const char *const FILE_TYPE_STRINGS[] = {
    "data",
    "empty",
    "ASCII text",
    "ISO-8859 text",
    "UTF-8 Unicode text",
    "very short file (no magic)"
};

enum file_type identify_file_type(const unsigned char *bytes, size_t length)
{
  if (length == 0)
    return EMPTY;
  if (length <= 1)
    return VERY_SHORT;

  // Check ASCII (allowed ranges: 0x07–0x0D, 0x1B, 0x20–0x7E)
  size_t i = 0;
  for (i = 0; i < length; i++)
  {
    unsigned char c = bytes[i];
    if (!((c >= 0x07 && c <= 0x0D) || c == 0x1B || (c >= 0x20 && c <= 0x7E)))
    {
      break;
    }
    if (i == length - 1)
      return ASCII;
  }

  // Check UTF-8 validity, but reject null bytes (0x00)
  i = 0;
  while (i < length)
  {
    unsigned char c = bytes[i];
    if (c == 0x00)
    {
      return DATA;
    }
    if (c <= 0x7F)
    {
      i++;
    }
    else if ((c >> 5) == 0x6 && i + 1 < length &&
             (bytes[i + 1] >> 6) == 0x2)
    {
      i += 2;
    }
    else if ((c >> 4) == 0xE && i + 2 < length &&
             (bytes[i + 1] >> 6) == 0x2 &&
             (bytes[i + 2] >> 6) == 0x2)
    {
      i += 3;
    }
    else if ((c >> 3) == 0x1E && i + 3 < length &&
             (bytes[i + 1] >> 6) == 0x2 &&
             (bytes[i + 2] >> 6) == 0x2 &&
             (bytes[i + 3] >> 6) == 0x2)
    {
      i += 4;
    }
    else
    {
      break; // invalid UTF-8
    }
  }
  if (i == length)
    return UTF8;

  // Check ISO-8859 (exclude 128–159)
  for (i = 0; i < length; i++)
  {
    unsigned char c = bytes[i];
    if (c >= 128 && c <= 159)
    {
      break;
    }
    if (i == length - 1)
      return ISO_8859;
  }

  // Otherwise, just call it data
  return DATA;
}

int print_error(char *path, int errnum)
{
  return fprintf(stdout, "%s: cannot determine (%s)\n", path, strerror(errnum));
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: file path\n");
    return EXIT_FAILURE;
  }

  FILE *f = fopen(argv[1], "rb");
  if (!f)
  {
    print_error(argv[1], errno);
    return EXIT_SUCCESS;
  }

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  unsigned char *buffer = malloc(size > 0 ? size : 1);
  if (!buffer)
  {
    fclose(f);
    fprintf(stderr, "Memory allocation failed\n");
    return EXIT_FAILURE;
  }

  fread(buffer, 1, size, f);
  fclose(f);

  enum file_type t = identify_file_type(buffer, size);
  printf("%s: %s\n", argv[1], FILE_TYPE_STRINGS[t]);

  free(buffer);
  return EXIT_SUCCESS;
}