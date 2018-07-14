#ifndef _NRT_UTIL_H_
#define _NRT_UTIL_H_

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

enum LogLevel {
  Normal,
  Verbose0,
  Verbose1,
  Verbose2
};

// extern verbosity;

// int verbosity = 0;

const char *filename_ext(const char *filename);

// void vlog(int level, const char *format, ...);

#endif
