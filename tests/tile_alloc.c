#include "nrt.h"
#include <assert.h>

int main() {
  nrt_tile_t *tile = NRT_TILE_ALLOC;

  assert(tile);

  free(tile);
}

