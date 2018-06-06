#ifndef _NRT_PRG_H_
#define _NRT_PRG_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "header.h"

#define NRT_PRG_BANK_SIZE 16384
#define NRT_PRG_BANK_MAX_COUNT 64

#define NRT_PRG_ALLOC (nrt_prgbank*)malloc(sizeof(nrt_prgbank))

typedef struct nrt_prgbank {
  unsigned char data[NRT_PRG_BANK_SIZE];
} nrt_prgbank;

int nrt_prg_offset(int prg_index);
bool nrt_prg_index_valid(nrt_header *header, int prg_index);
int nrt_extract_prg(FILE *rom, int index, nrt_prgbank *prg);

#endif
