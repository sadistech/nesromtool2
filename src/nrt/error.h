#ifndef _NRT_ERROR_H_
#define _NRT_ERROR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define NRT_MAX_ERRORS 16

#define NRT_ERROR_ALLOC (nrt_error*)malloc(sizeof(nrt_error))

typedef struct nrt_error {
  int count;
  char *errors[NRT_MAX_ERRORS];
} nrt_error;

void nrt_error_init(nrt_error *error);
bool nrt_is_error(nrt_error *error);
void nrt_append_error(nrt_error *error, char *msg);

#endif
