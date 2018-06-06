#include "nrt.h"
#include <assert.h>

int main() {
  nrt_tile *tile = NRT_TILE_ALLOC;

  //                  0 2 4 6
  //                  | | | |
  tile->chan_a[0] = 0b00001111;
  tile->chan_b[0] = 0b11000011;

  assert( nrt_get_tile_pixel(0, 0, tile) == 2 );
  assert( nrt_get_tile_pixel(0, 2, tile) == 0 );
  assert( nrt_get_tile_pixel(0, 4, tile) == 1 );
  assert( nrt_get_tile_pixel(0, 6, tile) == 3 );

  free(tile);
}

