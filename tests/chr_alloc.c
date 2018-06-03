#include <assert.h>
#include "nrt.h"

int main() {
  nrt_chrbank_t *chr = NRT_CHR_ALLOC;

  assert(chr);
  free(chr);
}

