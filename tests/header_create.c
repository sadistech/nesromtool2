#include <assert.h>
#include "nrt.h"

int main() {
  nrt_header* header = nrt_header_create(1, 2);

  assert( nrt_validate_header_magic_word(header) );
  assert( header->prg_count == 1 );
  assert( header->chr_count == 2 );
}
