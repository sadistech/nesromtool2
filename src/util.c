#include "util.h"

// from https://stackoverflow.com/questions/5309471/getting-file-extension-in-c
const char *filename_ext(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if(!dot || dot == filename) return "";
  return dot + 1;
}

char *filename_no_ext(const char *filename) {
  char *newfilename = (char*)calloc(1, strlen(filename + 1));
  strcpy(newfilename, filename);
  char *dot = strrchr(newfilename, '.');

  if (!dot || dot == newfilename) return newfilename;

  *dot = '\0';

  return newfilename;
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
