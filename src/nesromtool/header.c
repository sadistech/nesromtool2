#include "header.h"

bool nrt_validate_header_magic_word(nesheader_t* header) {
  return strncmp(header->magic_word, NES_MAGIC_WORD, NES_MAGIC_WORD_SIZE) == 0;
}

bool nrt_validate_header_prg_count(nesheader_t* header) {
  return header->prg_count > 0 && header->prg_count <= NES_PRG_BANK_MAX_COUNT;
}


