#ifndef _NESROMTOOL_CHR_H_
#define _NESROMTOOL_CHR_H_

#define NES_CHR_BANK_SIZE 8192
#define NES_CHR_TILE_SIZE 16
#define NES_CHR_TILE_COUNT 512 /* NES_CHR_BANK_SIZE / NES_CHR_TILE_SIZE */

#define nes_chr_offset(header, chr_index) NES_HEADER_SIZE + (header->prg_count * NES_PRG_BANK_SIZE) + (chr_index * NES_CHR_BANK_SIZE)

typedef struct neschrbank_t {
  unsigned char tile[NES_CHR_TILE_COUNT][NES_CHR_TILE_SIZE];
} neschrbank_t;

#endif
