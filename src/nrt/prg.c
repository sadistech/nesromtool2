#include "prg.h"

int nrt_prg_offset(int prg_index) {
  return NRT_HEADER_SIZE + (NRT_PRG_BANK_SIZE * prg_index);
}

bool nrt_prg_index_valid(nrt_header_t *header, int prg_index) {
  return (prg_index >= 0 && header->prg_count < prg_index + 1);
}

int nrt_extract_prg(FILE *rom, int index, nrt_prgbank_t *prg) {
  int offset = nrt_prg_offset(index);

  fseek(rom, offset, SEEK_SET);

  return fread(prg, sizeof(nrt_prgbank_t), 1, rom);
}
