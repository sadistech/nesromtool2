#include "chr.h"

int nrt_extract_chr(FILE *rom, int index, nrt_chrbank_t *chr) {
  nrt_header_t *header = NRT_HEADER_ALLOC;
  if (nrt_header_extract(rom, header) != 1) {
    fprintf(stderr, "Failed to extract header.\n");
    return 0;
  }

  if ((index - 1) > header->chr_count) {
    fprintf(stderr, "CHR index is out of bounds (%d > %d)", index, header->chr_count);
    return 0;
  }

  int offset = nrt_chr_offset(header, index);

  fprintf(stderr, "Freeing header\n");
  free(header);

  fprintf(stderr, "Moving to offset: %d\n", offset);
  fseek(rom, offset, SEEK_SET);

  fprintf(stderr, "Ready to read.\n");

  return fread(chr, sizeof(nrt_chrbank_t), 1, rom);
}
