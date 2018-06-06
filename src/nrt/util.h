#ifndef _NRT_UTIL_H_
#define _NRT_UTIL_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#include "error.h"
#include "header.h"

#define nrt_diec(code, f, ...) fprintf(stderr, f"\n", ##__VA_ARGS__); exit(code)
#define nrt_die(f, ...) nrt_diec(EXIT_FAILURE, f, ##__VA_ARGS__)

void nrt_validate_file(FILE *rom, nrt_error *status);

#endif

