#include "error.h"

bool nrt_is_error(nrt_error_t *error) {
  return error->count > 0;
}

void nrt_append_error(nrt_error_t *error, char *msg) {
  if (error->count >= NRT_MAX_ERRORS) {
    fprintf(stderr, "Cannot add additional errors.\n");
    exit(EXIT_FAILURE);
  }

  int len = strlen(msg);

  error->errors[error->count] = (char*)malloc(len + 1);
  strcpy(error->errors[error->count], msg);
  error->count++;
}

