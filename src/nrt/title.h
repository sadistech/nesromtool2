#ifndef _NRT_TITLE_H_
#define _NRT_TITLE_H_

#include <stdio.h>
#include "header.h"
#include "prg.h"
#include "chr.h"

#define NRT_TITLE_MAX_LENGTH 255

int nrt_read_title_from_file(FILE* rom, nrt_header* header, char* buf);
int nrt_write_title_to_file(FILE* rom, nrt_header* header, char* title);

#endif
