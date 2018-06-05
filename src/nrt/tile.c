#include "tile.h"

// tile:
// A0 A1 A2 A3 A4 A5 A6 A7  |  B0 B1 B2 B3 B4 B5 B6 B7

bool nrt_tile_to_bitmap(nrt_tile_t *tile, nrt_tile_bitmap_t *bitmap_buf) {
  int i, j;
  char channel_a;
  char channel_b;

/*  for (i = 0; i < NRT_TILE_CHANNEL_SIZE; i++) {
    // now, let's iterate over the bits in the row and write the pixels
    for (j = 0; j < 8; j++) {
      int pixel_offset = (i * NRT_TILE_WIDTH_PX) + (8 - j);
      printf("parsing %d\n", pixel_offset);
      bitmap_buf[pixel_offset] = nrt_tile_bits_to_pixel(tile->chan_a << j & 1, tile->chan_b << j & 1)
    }
  }
  */
  return true;
}

bool nrt_bitmap_to_tile(nrt_tile_bitmap_t *bitmap_buf, nrt_tile_t *tile) {
  return true;
}

int nrt_get_tile_pixel(unsigned int row, unsigned int col, nrt_tile_t *tile) {
  return nrt_tile_bits_to_pixel(
      NRT_GET_BIT(col, tile->chan_a[row]),
      NRT_GET_BIT(col, tile->chan_b[row])
  );
}

int nrt_tile_bits_to_pixel(int a, int b) {
  int a_val = a ? 1 : 0;
  int b_val = b ? 2 : 0;

  return a_val + b_val;
}
