#ifndef _NESROMTOOL_HEADER_H_
#define _NESROMTOOL_HEADER_H_

#include <stdbool.h>
#include <string.h>
#include "prg.h"
#include "chr.h"

#define NES_HEADER_SIZE 16
#define NES_MAGIC_WORD "NES\x1a"
#define NES_MAGIC_WORD_SIZE 4

#define NRT_HEADER_ALLOC (nesheader_t*)malloc(sizeof(nesheader_t))

typedef struct nesheader_t {
  char magic_word[NES_MAGIC_WORD_SIZE];
  unsigned char prg_count;
  unsigned char chr_count;
  unsigned char the_rest[10];
} nesheader_t;

bool nrt_validate_header_magic_word(nesheader_t* header);
bool nrt_validate_header_prg_count(nesheader_t* header);

#endif
