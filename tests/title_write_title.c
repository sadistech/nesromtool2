#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "nrt.h"
#include "test.h"

#define NEW_TITLE "nrt is testing a title"

int main() {
  test_init();

	char *inrom_path = path_join(fixture_dir, "notitle.nes");
	char *outrom_path = path_join(temp_dir, "adding-title.nes");

	assert(copy_file(outrom_path, inrom_path) == 0);

	free(inrom_path);

  FILE *rom = fopen(outrom_path, "r+");

  assert(rom);

  nrt_header *header = NRT_HEADER_ALLOC;
  nrt_header_extract(rom, header);

  // there better not be a title in there
  char title[NRT_TITLE_MAX_LENGTH];
  nrt_read_title_from_file(rom, header, title);
  assert(title[0] == '\0');

  nrt_write_title_to_file(rom, header, NEW_TITLE);

  fclose(rom);
	free(header);

  bzero(title, NRT_TITLE_MAX_LENGTH);

  // ok, let's read it back in, now and test it.
	rom = fopen(outrom_path, "r");

  assert(rom);

  header = NRT_HEADER_ALLOC;
  nrt_header_extract(rom, header);

  nrt_read_title_from_file(rom, header, title);

  printf("title: %s\n", title);

  assert(strcmp(title, NEW_TITLE) == 0);

  fclose(rom);
  free(header);
	free(outrom_path);

}
