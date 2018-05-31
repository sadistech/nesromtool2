#include "chr.h"

int nrt_extract_chr(FILE *rom, int index, nrt_chrbank_t *chr) {
  nrt_header_t *header = NRT_HEADER_ALLOC;
  if (nrt_header_extract(rom, header) != 1) {
    nrt_die("Failed to extract header.");
  }

  if ((index - 1) > header->chr_count) {
    nrt_die("CHR index is out of bounds (%d > %d)", index, header->chr_count);
  }

  int offset = nrt_chr_offset(header, index);

  free(header);

  fseek(rom, offset, SEEK_SET);

  return fread(chr, sizeof(nrt_chrbank_t), 1, rom);
}
