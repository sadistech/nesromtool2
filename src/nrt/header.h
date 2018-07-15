#ifndef _NRT_HEADER_H_
#define _NRT_HEADER_H_

#include <stdbool.h>
#include <string.h>
#include "util.h"

#define NRT_HEADER_SIZE 16
#define NRT_MAGIC_WORD "NES\x1a"
#define NRT_MAGIC_WORD_SIZE 4

#define NRT_HEADER_ALLOC (nrt_header*)malloc(sizeof(nrt_header))

typedef enum tv_system {
  tv_system_ntsc = 0,
  tv_system_pal = 1
} tv_system;

typedef struct nrt_header {
  char magic_word[NRT_MAGIC_WORD_SIZE];
  unsigned char prg_count;
  unsigned char chr_count;

  // flags 6
  unsigned char mapper_nybble_lower : 4;
  unsigned char ignore_mirroring_control : 1;
  unsigned char trainer_present : 1;
  unsigned char persistent_memory : 1;
  unsigned char mirror_mode : 1;

  // flags 7
  unsigned char mapper_nybble_upper : 4;
  unsigned char flag_format : 2;
  unsigned char playchoice : 1;
  unsigned char vs_unisystem : 1;

  // flags 8
  unsigned char prg_ram_size;

  // flags 9
  unsigned char reserved : 7;
  unsigned char tv_system : 1;

  // flags 10
  unsigned char unknown_1 : 2;
  unsigned char bus_conflicts : 1;
  unsigned char prg_ram_absent : 1;
  unsigned char unknown_2 : 2;
  unsigned char tv_system_2 : 2; // 00 = NTSC, 10 = PAL, 01/11 = both?

  // zero filled
  unsigned char the_rest[5];
} nrt_header;

nrt_header* nrt_header_create(int prg_count, int chr_count);
int nrt_header_extract(FILE *rom, nrt_header *header);
bool nrt_validate_header(nrt_header *header);
bool nrt_validate_header_magic_word(nrt_header* header);
bool nrt_validate_header_prg_count(nrt_header* header);

#endif
