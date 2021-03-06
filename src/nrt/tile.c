#include "tile.h"

// tile:
// A0 A1 A2 A3 A4 A5 A6 A7  |  B0 B1 B2 B3 B4 B5 B6 B7

void nrt_tile_to_bitmap(nrt_tile *tile, nrt_tile_bitmap *bitmap) {
  int i, j;

  // iterate over each byte (row of pixels)
  for (i = 0; i < NRT_TILE_CHANNEL_SIZE; i++) {
    // now, let's iterate over the bits in the row and write the pixels
    for (j = 0; j < 8; j++) {
      int pixel_offset = (i * NRT_TILE_WIDTH_PX) + j;
      bitmap->pixels[pixel_offset] = nrt_get_tile_pixel(i, j, tile);
    }
  }
}

void nrt_bitmap_to_tile(nrt_tile_bitmap *bitmap, nrt_tile *tile) {
  // iterate over the bitmap pixels
  int x, y;
  int pixel;

  for (y = 0; y < NRT_TILE_HEIGHT_PX; y++) {
    for (x = 0; x < NRT_TILE_WIDTH_PX; x++) {
      // shift the current byte over first
      tile->chan_a[y] <<= 1;
      tile->chan_b[y] <<= 1;

      // write the bit to each channel
      pixel = bitmap->pixels[x + (y * NRT_TILE_WIDTH_PX)];
      tile->chan_a[y] += pixel & 1;
      tile->chan_b[y] += pixel >> 1 & 1;
    }
  }
}

bool nrt_tiles_to_png(nrt_tile_bitmap *tiles, int count, int width, FILE *outfile) {
  int height = count / width;

  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info_ptr = png_create_info_struct(png_ptr);

  if (!info_ptr) {
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    return false;
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return false;
  }

  // set up the palette.
  static const png_color palette[] = {
    {0x00, 0x00, 0x00},
    {0xFF, 0x00, 0x00},
    {0xFF, 0xFF, 0x00},
    {0x00, 0x00, 0xFF}
  };

  // 16w x 32h tiles
  png_byte *row = (png_byte*)malloc(width * (NRT_TILE_WIDTH_PX / NRT_TILE_BITMAP_PALETTE_SIZE) * sizeof(png_byte));

  png_init_io(png_ptr, outfile);
  //png_set_write_status_fn(png_ptr, write_row_callback);

  png_set_IHDR(png_ptr, info_ptr, width * NRT_TILE_WIDTH_PX, height * NRT_TILE_WIDTH_PX, NRT_TILE_BITMAP_BITDEPTH, PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  png_set_PLTE(png_ptr, info_ptr, palette, NRT_TILE_BITMAP_PALETTE_SIZE);

  png_write_info(png_ptr, info_ptr);

  int tile_row = 0;
  int tile_idx = 0;
  int idx = 0;
  int i;
  int j;

  // iterate over each row of tiles
  for (tile_row = 0; tile_row < height; tile_row++) {
    // iterate over each pixel in the row
    for (i = 0; i < NRT_TILE_WIDTH_PX; i++) {
      bzero(row, width * 2 * sizeof(png_byte));
      //iterate over each row of pixels in the tile
      for(j = 0; j < NRT_TILE_HEIGHT_PX; j++) {
        // iterate over each tile in the row
        for (tile_idx = 0; tile_idx < width; tile_idx++) {
          idx = 0;
          if (j >3) {
            idx = 1;
          }

          idx += (tile_idx * 2);
          row[idx] = row[idx] << 2;
          row[idx] += tiles[tile_row * width + tile_idx].pixels[i * NRT_TILE_HEIGHT_PX + j];
        }
      }
      png_write_row(png_ptr, row);
    }
  }

  png_write_end(png_ptr, NULL);

  if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
  free(row);

  return true;
}

bool nrt_png_to_tiles(FILE *png_file, nrt_tile_bitmap_lockup *lockup) {
  char header[NRT_PNG_SIG_SIZE];

  fseek(png_file, 0, SEEK_SET);
  fread(header, 1, NRT_PNG_SIG_SIZE, png_file);

  if (png_sig_cmp((png_const_bytep)header, 0, 8)) {
    fprintf(stderr, "this is not a png!\n");
    return false;
  }

  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr) {
    fprintf(stderr, "Failed to allocate png_ptr\n");
    return false;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);

  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    return false;
  }

  png_infop end_info = png_create_info_struct(png_ptr);
  if (!end_info) {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    return false;
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    return false;
  }

  png_init_io(png_ptr, png_file);
  png_set_sig_bytes(png_ptr, NRT_PNG_SIG_SIZE);
  png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_PACKING, NULL);

  //png_read_info(png_ptr, info_ptr);
  int width = png_get_image_width(png_ptr, info_ptr);
  int height = png_get_image_height(png_ptr, info_ptr);
  int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  int color_type = png_get_color_type(png_ptr, info_ptr);

  if ( color_type != PNG_COLOR_TYPE_PALETTE ) {
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    return false;
  }

  int tiles_width = width / NRT_TILE_WIDTH_PX;
  int tiles_height = height / NRT_TILE_HEIGHT_PX;
  int tiles_count = tiles_width * tiles_height;

  // configure the lockup and allocate the memory
  lockup->count = tiles_count;
  lockup->width = tiles_width;
  lockup->bitmaps = (nrt_tile_bitmap*)calloc(tiles_count, sizeof(nrt_tile_bitmap));

  /* png_bytep row = (png_bytep)malloc(tiles_width * NRT_TILE_WIDTH_PX * bit_depth / CHAR_BIT * sizeof(png_byte)); */
  png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
  png_bytep row;

  nrt_tile_bitmap *current_tile = NULL;

  // iterate over each row of tiles
  // each row of pixels
  // each tile in the row
  // each pixel in the tile
  int tile_row; // which row of tiles we're on
  int px_row;   // which row of pixels we're on for the row of tiles
  int tile;     // which column of tiles we're on
  int tile_px;  // which pixel we're on in the row of pixels

  png_byte px; // our scratch pad for the value of the current pixel

  for (tile_row = 0; tile_row < tiles_height; tile_row++) {
    for (px_row = 0; px_row < NRT_TILE_HEIGHT_PX; px_row++) {
      row = row_pointers[tile_row * NRT_TILE_HEIGHT_PX + px_row];

      for (tile = 0; tile < tiles_width; tile++) {
        for (tile_px = 0; tile_px < NRT_TILE_WIDTH_PX; tile_px++) {
          // get the current byte from the png row
          px = row[tile * NRT_TILE_WIDTH_PX + tile_px];

          current_tile = &lockup->bitmaps[tile_row * tiles_width + tile];
          current_tile->pixels[px_row * NRT_TILE_WIDTH_PX + tile_px] = (char)px;
        }
      }
    }
  }

  return true;
}

int nrt_get_tile_pixel(unsigned int row, unsigned int col, nrt_tile *tile) {
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

