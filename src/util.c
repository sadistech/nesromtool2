#include "util.h"

// from https://stackoverflow.com/questions/5309471/getting-file-extension-in-c
const char *filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

