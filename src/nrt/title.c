#include "title.h"

int nrt_read_title_from_file(FILE* rom, nrt_header* header, char* buf) {
  // first seek to end of file
  int offset = NRT_HEADER_SIZE + (header->prg_count * NRT_PRG_BANK_SIZE) + (header->chr_count * NRT_CHR_BANK_SIZE);

  fseek(rom, offset, SEEK_SET);

  return fread(buf, NRT_TITLE_MAX_LENGTH, 1, rom);
}

int nrt_write_title_to_file(FILE* rom, nrt_header* header, char* title) {
  int offset = NRT_HEADER_SIZE + (header->prg_count * NRT_PRG_BANK_SIZE) + (header->chr_count * NRT_CHR_BANK_SIZE);

  fseek(rom, offset, SEEK_SET);

  return fwrite(title, NRT_TITLE_MAX_LENGTH, 1, rom);
}

