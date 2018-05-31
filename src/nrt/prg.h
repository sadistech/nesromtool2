#ifndef _NRT_PRG_H_
#define _NRT_PRG_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "header.h"

#define NRT_PRG_BANK_SIZE 16384
#define NRT_PRG_BANK_MAX_COUNT 64

#define NRT_PRG_ALLOC (nrt_prgbank_t*)malloc(sizeof(nrt_prgbank_t))

typedef struct nrt_prgbank_t {
  unsigned char data[NRT_PRG_BANK_SIZE];
} nrt_prgbank_t;

int nrt_prg_offset(int prg_index);
bool nrt_prg_index_valid(nrt_header_t *header, int prg_index);
int nrt_extract_prg(FILE *rom, int index, nrt_prgbank_t *prg);

#endif
