#include "prg.h"

int nrt_extract_prg(FILE *rom, int index, nrt_prgbank_t *prg) {
  nrt_header_t *header = NRT_HEADER_ALLOC;
  if (nrt_header_extract(rom, header) != 1) {
    free(header);
    nrt_die("Failed to extract header.");
  }

  if ((index - 1) > header->prg_count) {
    free(header);
    nrt_die("PRG index is out of bounds (%d > %d)", index, header->prg_count);
  }

  int offset = nrt_prg_offset(index);

  free(header);

  fseek(rom, offset, SEEK_SET);

  return fread(prg, sizeof(nrt_prgbank_t), 1, rom);
}
