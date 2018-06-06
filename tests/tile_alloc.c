#include "nrt.h"
#include <assert.h>

int main() {
  nrt_tile *tile = NRT_TILE_ALLOC;

  assert(tile);

  free(tile);
}

