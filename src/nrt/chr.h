#ifndef _NRT_CHR_H_
#define _NRT_CHR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "header.h"

#define NRT_CHR_BANK_SIZE 8192
#define NRT_CHR_TILE_SIZE 16
#define NRT_CHR_TILE_COUNT 512 /* NRT_CHR_BANK_SIZE / NRT_CHR_TILE_SIZE */

#define NRT_CHR_ALLOC (nrt_chrbank_t*)malloc(sizeof(nrt_chrbank_t))

typedef struct nrt_chrbank_t {
  unsigned char tile[NRT_CHR_TILE_COUNT][NRT_CHR_TILE_SIZE];
} nrt_chrbank_t;

int nrt_chr_offset(nrt_header_t *header, int chr_index);
bool nrt_chr_index_valid(nrt_header_t *header, int chr_index);
int nrt_extract_chr(FILE *rom, nrt_header_t *header, int index, nrt_chrbank_t *chr);

#endif
