#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "util.h"

int main() {
  char *filename_w_no_ext = "foo";
  char *filename_with_ext = "foo.txt";
  char *filename_with_dots = "foo.bar.txt";

  char *ext_filename_no_ext = filename_no_ext(filename_w_no_ext);
  char *ext_filename_with_ext = filename_no_ext(filename_with_ext);
  char *ext_filename_with_dots = filename_no_ext(filename_with_dots);

  assert(strcmp(ext_filename_no_ext, filename_w_no_ext) == 0);
  assert(strcmp(ext_filename_with_ext, "foo") == 0);
  assert(strcmp(ext_filename_with_dots, "foo.bar") == 0);

  free(ext_filename_no_ext);
  free(ext_filename_with_ext);
  free(ext_filename_with_dots);
}


