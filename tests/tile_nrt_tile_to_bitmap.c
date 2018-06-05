#include "nrt.h"
#include <assert.h>

int main() {
  nrt_tile_t *tile = NRT_TILE_ALLOC;
  nrt_tile_bitmap_t *bitmap = NRT_TILE_BITMAP_ALLOC;

  assert(tile);
  assert(bitmap);

  bzero(tile->chan_a, NRT_TILE_CHANNEL_SIZE);
  bzero(tile->chan_b, NRT_TILE_CHANNEL_SIZE);

  tile->chan_a[0] = 0b11110000;
  tile->chan_a[1] = 0b00000000;
  tile->chan_a[2] = 0b11110000;
  tile->chan_a[3] = 0b00000000;
  tile->chan_a[4] = 0b11110000;
  tile->chan_a[5] = 0b00000000;
  tile->chan_a[6] = 0b11110000;
  tile->chan_a[7] = 0b00000000;

  tile->chan_b[0] = 0b00111100;
  tile->chan_b[1] = 0b00000000;
  tile->chan_b[2] = 0b00111100;
  tile->chan_b[3] = 0b00000000;
  tile->chan_b[4] = 0b00111100;
  tile->chan_b[5] = 0b00000000;
  tile->chan_b[6] = 0b00111100;
  tile->chan_b[7] = 0b00000000;

  nrt_tile_to_bitmap(tile, bitmap);

  char expected_bitmap[] = {
    1, 1, 3, 3, 2, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 3, 3, 2, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 3, 3, 2, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 3, 3, 2, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  };

  assert( memcmp(bitmap, expected_bitmap, NRT_TILE_BITMAP_SIZE) == 0 );
}
