#include "util.h"

// from https://stackoverflow.com/questions/5309471/getting-file-extension-in-c
const char *filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

/* void vlog(int level, const char *format, ...) { */
  /* va_list args; */
  /* va_start(args, format); */

  /* if (level < verbosity) { */
    /* return; */
  /* } */

  /* fprintf(stderr, format, args); */

  /* va_end(args); */
/* } */
