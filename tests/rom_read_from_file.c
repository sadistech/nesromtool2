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

  assert( rom->prgs->count == 1 );
  assert( rom->chrs->count == 2 );

  assert( strcmp((char*)rom->prgs->list->prg, "PRG 00") == 0 );
  assert( strcmp((char*)rom->chrs->list->chr, "CHR 00") == 0 );
  assert( strcmp((char*)rom->chrs->list->next->chr, "CHR 01") == 0 );

  assert( strcmp(rom->title, "TITLE") == 0 );

  fclose(romfile);
  nrt_rom_free(rom);
}
