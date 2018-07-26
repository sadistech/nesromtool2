#ifndef _UTIL_H_
#define _UTIL_H_

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: use PATH_MAX from sys/limits.h here
#define PATH_LENGTH 1024

enum LogLevel {
  Normal,
  Verbose0,
  Verbose1,
  Verbose2
};

// extern verbosity;

// int verbosity = 0;

const char *filename_ext(const char *filename);
char *filename_no_ext(const char *filename);

// void vlog(int level, const char *format, ...);

#endif
