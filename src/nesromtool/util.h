#ifndef _NESROMTOOL_UTIL_H_
#define _NESROMTOOL_UTIL_H_

#include "header.h"

typedef struct nesrom_status_t {
  bool is_valid;
  char **errors;
} nesrom_status_t;

void nesromtool_validate_file(FILE *rom, nesrom_status_t *status);

#endif

