#ifndef _NRT_TILE_H_
#define _NRT_TILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NRT_TILE_WIDTH_PX 8
#define NRT_TILE_HEIGHT_PX 8
#define NRT_TILE_SIZE 16

#define NRT_TILE_CHANNEL_SIZE 8

#define NRT_TILE_ALLOC (nrt_tile*)malloc(sizeof(nrt_tile))

#define NRT_TILE_BITMAP_SIZE (NRT_TILE_WIDTH_PX * NRT_TILE_WIDTH_PX)

#define NRT_TILE_BITMAP_ALLOC (nrt_tile_bitmap*)malloc(sizeof(nrt_tile))

#define NRT_GET_BIT(bit_index, row_bits) (row_bits >> (7 - bit_index) & 1)

typedef struct nrt_tile {
  char chan_a[NRT_TILE_CHANNEL_SIZE];
  char chan_b[NRT_TILE_CHANNEL_SIZE];
} nrt_tile;

typedef struct nrt_tile_bitmap {
  char pixels[NRT_TILE_BITMAP_SIZE];
} nrt_tile_bitmap;

void nrt_tile_to_bitmap(nrt_tile *tile, nrt_tile_bitmap *bitmap_buf);
void nrt_bitmap_to_tile(nrt_tile_bitmap *bitmap_buf, nrt_tile *tile);

int nrt_get_tile_pixel(unsigned int row, unsigned int col, nrt_tile *tile);
int nrt_tile_bits_to_pixel(int a, int b);

#endif
