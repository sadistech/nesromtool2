#include <assert.h>
#include "nrt.h"

int main() {
  nrt_header *header = NRT_HEADER_ALLOC;
  strncpy(header->magic_word, "FOOD", NRT_MAGIC_WORD_SIZE);
  header->prg_count = 1;
  header->chr_count = 1;

  assert(!nrt_validate_header(header));

  free(header);
}

