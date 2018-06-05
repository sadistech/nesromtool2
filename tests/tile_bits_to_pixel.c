#include "nrt.h"
#include <assert.h>

int main() {
  assert( nrt_tile_bits_to_pixel(0, 0) == 0 );
  assert( nrt_tile_bits_to_pixel(0, 1) == 2 );
  assert( nrt_tile_bits_to_pixel(1, 0) == 1 );
  assert( nrt_tile_bits_to_pixel(1, 1) == 3 );
}
