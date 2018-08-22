#ifndef _NRT_CHR_H_
#define _NRT_CHR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "header.h"
#include "tile.h"

#define NRT_CHR_BANK_SIZE 8192
#define NRT_CHR_TILE_COUNT 512 /* NRT_CHR_BANK_SIZE / NRT_TILE_SIZE */

#define NRT_CHR_ALLOC (nrt_chrbank*)calloc(1, sizeof(nrt_chrbank))

typedef struct nrt_chrbank {
  nrt_tile tile[NRT_CHR_TILE_COUNT];
} nrt_chrbank;

int nrt_chr_offset(nrt_header *header, int chr_index);
bool nrt_chr_index_valid(nrt_header *header, int chr_index);
int nrt_extract_chr(FILE *rom, nrt_header *header, int index, nrt_chrbank *chr);
int nrt_replace_chr(FILE *rom, nrt_header *header, int index, nrt_chrbank *chr);

bool nrt_chr_valid_filesize(struct stat *filestat);

#endif
