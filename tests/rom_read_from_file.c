#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nrt.h"

int main() {
  char *fixture_dir = getenv("NRT_FIXTURE_DIR");

  char rompath[512];
  bzero(rompath, 512);

  sprintf(rompath, "%s/title.nes", fixture_dir);

  FILE* romfile = fopen(rompath, "r");

  assert(romfile);

  nrt_rom* rom = nrt_read_rom_from_file(romfile);

  assert( rom->header->prg_count == 1 );
  assert( rom->header->chr_count == 2 );

  assert( nrt_prgbank_list_count(rom->prg_banks) == 1 );
  assert( nrt_chrbank_list_count(rom->chr_banks) == 2 );

  assert( strcmp((char*)rom->prg_banks->prg, "PRG 00") == 0 );
  assert( strcmp((char*)rom->chr_banks->chr, "CHR 00") == 0 );
  assert( strcmp((char*)rom->chr_banks->next->chr, "CHR 01") == 0 );

  assert( strcmp(rom->title, "TITLE") == 0 );
}
