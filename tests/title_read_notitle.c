#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "nrt.h"

int main() {
  char* fixture_dir = getenv("NRT_FIXTURE_DIR");

  char rom_path[512];
  sprintf(rom_path, "%s/notitle.nes", fixture_dir);

  FILE* rom = NULL;

  rom = fopen(rom_path, "r");

  nrt_header* header = NRT_HEADER_ALLOC;
  nrt_header_extract(rom, header);

  char title_buf[NRT_TITLE_MAX_LENGTH];

  assert( nrt_read_title_from_file(rom, header, title_buf) == 0 );

  fclose(rom);
}

