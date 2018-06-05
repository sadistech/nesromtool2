#include "nrt.h"
#include <assert.h>

int main() {
  nrt_tile_t *tile = NRT_TILE_ALLOC;
  nrt_tile_bitmap_t *bitmap = NRT_TILE_BITMAP_ALLOC;

  assert(tile);
  assert(bitmap);

  char pixels[] = {
    1, 1, 3, 3, 2, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 3, 3, 2, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 3, 3, 2, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 3, 3, 2, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  };

  memcpy(bitmap->pixels, pixels, NRT_TILE_WIDTH_PX * NRT_TILE_HEIGHT_PX);

  char expected_chan_a[NRT_TILE_CHANNEL_SIZE];
  char expected_chan_b[NRT_TILE_CHANNEL_SIZE];

  expected_chan_a[0] = 0b11110000;
  expected_chan_a[1] = 0b00000000;
  expected_chan_a[2] = 0b11110000;
  expected_chan_a[3] = 0b00000000;
  expected_chan_a[4] = 0b11110000;
  expected_chan_a[5] = 0b00000000;
  expected_chan_a[6] = 0b11110000;
  expected_chan_a[7] = 0b00000000;

  expected_chan_b[0] = 0b00111100;
  expected_chan_b[1] = 0b00000000;
  expected_chan_b[2] = 0b00111100;
  expected_chan_b[3] = 0b00000000;
  expected_chan_b[4] = 0b00111100;
  expected_chan_b[5] = 0b00000000;
  expected_chan_b[6] = 0b00111100;
  expected_chan_b[7] = 0b00000000;

  nrt_bitmap_to_tile(bitmap, tile);

  assert( memcmp(tile->chan_a, expected_chan_a, NRT_TILE_CHANNEL_SIZE) == 0 );
  assert( memcmp(tile->chan_b, expected_chan_b, NRT_TILE_CHANNEL_SIZE) == 0 );
}
