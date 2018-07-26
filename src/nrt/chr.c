#include "chr.h"
#include "prg.h"

int nrt_chr_offset(nrt_header *header, int chr_index) {
  return (NRT_HEADER_SIZE + (header->prg_count * NRT_PRG_BANK_SIZE)
      + (chr_index * NRT_CHR_BANK_SIZE));
}

bool nrt_chr_index_valid(nrt_header *header, int chr_index) {
  return (chr_index >= 0 && header->chr_count > chr_index);
}

int nrt_extract_chr(FILE *rom, nrt_header *header, int index, nrt_chrbank *chr) {
  int offset = nrt_chr_offset(header, index);

  fseek(rom, offset, SEEK_SET);

  return fread(chr, NRT_CHR_BANK_SIZE, 1, rom);
}

int nrt_replace_chr(FILE *rom, nrt_header *header, int index, nrt_chrbank *chr) {
  int offset = nrt_chr_offset(header, index);

  fseek(rom, offset, SEEK_SET);

  return fwrite(chr, NRT_CHR_BANK_SIZE, 1, rom);
}

bool nrt_chr_valid_filesize(struct stat *filestat) {
  return filestat->st_size == NRT_CHR_BANK_SIZE;
}
