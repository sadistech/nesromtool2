#include <assert.h>
#include "nrt.h"

int main() {
  nrt_header *header = NRT_HEADER_ALLOC;

  header->chr_count = 5;

  assert(nrt_chr_index_valid(header, 4));
  free(header);
}

