#include "header.h"

bool nrt_validate_header_magic_word(nrt_header_t* header) {
  return strncmp(header->magic_word, NRT_MAGIC_WORD, NRT_MAGIC_WORD_SIZE) == 0;
}

bool nrt_validate_header_prg_count(nrt_header_t* header) {
  return header->prg_count > 0 && header->prg_count <= NRT_PRG_BANK_MAX_COUNT;
}


