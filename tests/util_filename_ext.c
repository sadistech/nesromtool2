#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "util.h"

int main() {
  char *filename_w_no_ext = "foo";
  char *filename_with_ext = "foo.txt";
  char *filename_with_dots = "foo.bar.txt";

  const char *ext_filename_no_ext = filename_ext(filename_w_no_ext);
  const char *ext_filename_with_ext = filename_ext(filename_with_ext);
  const char *ext_filename_with_dots = filename_ext(filename_with_dots);

  assert(ext_filename_no_ext[0] == '\0');
  assert(strcmp(ext_filename_with_ext, "txt") == 0);
  assert(strcmp(ext_filename_with_dots, "txt") == 0);
}

