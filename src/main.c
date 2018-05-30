#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <libgen.h>

#include "nrt.h"

// verify <path>
void action_verify(char *filepath) {
  FILE *ifile = NULL;

  if (!(ifile = fopen(filepath, "r+"))) {
    fprintf(stderr, "Failed to open file.");
    exit(EXIT_FAILURE);
  }

  nrt_error_t *error = NRT_ERROR_ALLOC;

  nrt_validate_file(ifile, error);

  if (nrt_is_error(error)) {
    printf("%s: FAIL\n", basename(filepath));
    exit(EXIT_FAILURE);
  } else {
    printf("%s: OK\n", basename(filepath));
    exit(EXIT_SUCCESS);
  }
}

int main(int argc, char **argv, char **env) {
  char *appname = *(argv++);
  char *action = *(argv++);
  char *filepath = *(argv++);

  if (strcmp(action, "verify") == 0) {
    action_verify(filepath);
  } else {
    perror("No action.");
    exit(EXIT_FAILURE);
  }

  return 0;
}
