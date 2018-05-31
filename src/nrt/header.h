#ifndef _NRT_HEADER_H_
#define _NRT_HEADER_H_

#include <stdbool.h>
#include <string.h>
#include "util.h"

#define NRT_HEADER_SIZE 16
#define NRT_MAGIC_WORD "NES\x1a"
#define NRT_MAGIC_WORD_SIZE 4

#define NRT_HEADER_ALLOC (nrt_header_t*)malloc(sizeof(nrt_header_t))

typedef struct nrt_header_t {
  char magic_word[NRT_MAGIC_WORD_SIZE];
  unsigned char prg_count;
  unsigned char chr_count;
  unsigned char the_rest[10];
} nrt_header_t;

int nrt_header_extract(FILE *rom, nrt_header_t *header);
bool nrt_validate_header_magic_word(nrt_header_t* header);
bool nrt_validate_header_prg_count(nrt_header_t* header);

#endif
