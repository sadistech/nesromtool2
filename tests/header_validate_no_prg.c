#include <assert.h>
#include "nrt.h"

int main() {
  nrt_header_t *header = NRT_HEADER_ALLOC;
  strncpy(header->magic_word, NRT_MAGIC_WORD, NRT_MAGIC_WORD_SIZE);
  header->prg_count = 0;
  header->chr_count = 1;

  assert(!nrt_validate_header(header));

  free(header);
}

