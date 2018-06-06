#include "header.h"
#include "prg.h"

int nrt_header_extract(FILE *rom, nrt_header *header) {
  fseek(rom, 0, SEEK_SET);

  return fread(header, NRT_HEADER_SIZE, 1, rom);
}

bool nrt_validate_header(nrt_header *header) {
  return nrt_validate_header_magic_word(header) && nrt_validate_header_prg_count(header);
}

bool nrt_validate_header_magic_word(nrt_header* header) {
  return strncmp(header->magic_word, NRT_MAGIC_WORD, NRT_MAGIC_WORD_SIZE) == 0;
}

bool nrt_validate_header_prg_count(nrt_header* header) {
  return header->prg_count > 0 && header->prg_count <= NRT_PRG_BANK_MAX_COUNT;
}


