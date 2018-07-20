#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "nrt.h"
#include "test.h"

int main() {
  test_init();

  char *rom_path = path_join(fixture_dir, "title.nes");

  FILE* rom = fopen(rom_path, "r");

  assert(rom);

  nrt_header* header = NRT_HEADER_ALLOC;
  nrt_header_extract(rom, header);

  char title_buf[NRT_TITLE_MAX_LENGTH];
  char expected_title[] = "TITLE";

  assert( nrt_read_title_from_file(rom, header, title_buf) );
  assert( strcmp(title_buf, expected_title) == 0 );

  fclose(rom);
  free(rom_path);
}
