#ifndef _NRT_TILE_H_
#define _NRT_TILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NRT_TILE_WIDTH_PX 8
#define NRT_TILE_HEIGHT_PX 8
#define NRT_TILE_SIZE 16

#define NRT_TILE_CHANNEL_SIZE 8

#define NRT_TILE_ALLOC (nrt_tile_t*)malloc(sizeof(nrt_tile_t))

#define NRT_TILE_BITMAP_SIZE (NRT_TILE_WIDTH_PX * NRT_TILE_WIDTH_PX)

#define NRT_TILE_BITMAP_ALLOC (nrt_tile_bitmap_t*)malloc(sizeof(nrt_tile_t))

#define NRT_GET_BIT(bit_index, row_bits) (row_bits >> (7 - bit_index) & 1)

typedef struct nrt_tile_t {
  char chan_a[NRT_TILE_CHANNEL_SIZE];
  char chan_b[NRT_TILE_CHANNEL_SIZE];
} nrt_tile_t;

typedef struct nrt_tile_bitmap_t {
  char pixels[NRT_TILE_BITMAP_SIZE];
} nrt_tile_bitmap_t;

void nrt_tile_to_bitmap(nrt_tile_t *tile, nrt_tile_bitmap_t *bitmap_buf);
void nrt_bitmap_to_tile(nrt_tile_bitmap_t *bitmap_buf, nrt_tile_t *tile);

int nrt_get_tile_pixel(unsigned int row, unsigned int col, nrt_tile_t *tile);
int nrt_tile_bits_to_pixel(int a, int b);

#endif
