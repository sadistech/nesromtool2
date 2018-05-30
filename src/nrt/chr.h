#ifndef _NRT_CHR_H_
#define _NRT_CHR_H_

#define NRT_CHR_BANK_SIZE 8192
#define NRT_CHR_TILE_SIZE 16
#define NRT_CHR_TILE_COUNT 512 /* NRT_CHR_BANK_SIZE / NRT_CHR_TILE_SIZE */

#define nrt_chr_offset(header, chr_index) NRT_HEADER_SIZE + (header->prg_count * NRT_PRG_BANK_SIZE) + (chr_index * NRT_CHR_BANK_SIZE)

typedef struct nrt_chrbank_t {
  unsigned char tile[NRT_CHR_TILE_COUNT][NRT_CHR_TILE_SIZE];
} nrt_chrbank_t;

#endif
