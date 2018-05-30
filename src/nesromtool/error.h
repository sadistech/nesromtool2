#ifndef _NESROMTOOL_ERROR_H_
#define _NESROMTOOL_ERROR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define NRT_MAX_ERRORS 16

#define NRT_ERROR_ALLOC (nrt_error_t*)malloc(sizeof(nrt_error_t))

typedef struct nrt_error_t {
  int count;
  char *errors[NRT_MAX_ERRORS];
} nrt_error_t;

bool nrt_is_error(nrt_error_t *error);
void nrt_append_error(nrt_error_t *error, char *msg);

#endif
