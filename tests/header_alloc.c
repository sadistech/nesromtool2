#include <assert.h>
#include "nrt.h"

int main() {
  nrt_header *header = NRT_HEADER_ALLOC;

  assert(header);
  free(header);
}
