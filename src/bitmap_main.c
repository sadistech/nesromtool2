#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <limits.h> // for CHAR_BIT

#include "nrt.h"

/* #define PNG_FILE "chr1.png" */
#define PNG_FILE "chr1-edit.png"

int main() {
  FILE *infile = NULL;

  infile = fopen(PNG_FILE, "rb");

  if (!infile) {
    fprintf(stderr, "Error opening file.\n");
    exit(EXIT_FAILURE);
  }

  char header[8];
  fread(header, 1, 8, infile);

  printf("Read header.\n");

  if (png_sig_cmp((png_const_bytep)header, 0, 8)) {
    fprintf(stderr, "this is not a png!\n");
    fclose(infile);
    exit(EXIT_FAILURE);
  }
  printf("Header looks good..\n");

  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr) {
    fprintf(stderr, "Failed to allocate png_ptr\n");
    fclose(infile);
    exit(EXIT_FAILURE);
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);

  if (!info_ptr) {
    fclose(infile);
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    return(EXIT_FAILURE);
  }

  png_infop end_info = png_create_info_struct(png_ptr);
  if (!end_info) {
    fclose(infile);
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    exit(EXIT_FAILURE);
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(infile);
    exit(EXIT_FAILURE);
  }

  png_init_io(png_ptr, infile);
  png_set_sig_bytes(png_ptr, 8);
  png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_PACKING, NULL);

  //png_read_info(png_ptr, info_ptr);
  int width = png_get_image_width(png_ptr, info_ptr);
  int height = png_get_image_height(png_ptr, info_ptr);
  int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  int color_type = png_get_color_type(png_ptr, info_ptr);
  char *color_type_str = NULL;

  switch (color_type) {
    case PNG_COLOR_TYPE_PALETTE:
      color_type_str = "indexed";
      break;
    case PNG_COLOR_TYPE_RGB:
      color_type_str = "rgb";
      break;
    default:
      color_type_str = "other";
  }

  fprintf(stderr, "Image is %d x %d (%d-bit)\n", width, height, bit_depth);
  fprintf(stderr, "Color type: %s\n", color_type_str);

  // FIXME: don't use assert here.
  assert(color_type == PNG_COLOR_TYPE_PALETTE);

  int tiles_width = width / 8;
  int tiles_height = height / 8;
  int tiles_count = tiles_width * tiles_height;

  /* png_bytep row = (png_bytep)malloc(tiles_width * NRT_TILE_WIDTH_PX * bit_depth / CHAR_BIT * sizeof(png_byte)); */
  png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
  png_bytep row;

  // allocate all of our memory for all of the tiles
  nrt_tile_bitmap *tiles = (nrt_tile_bitmap*)malloc(tiles_count * sizeof(nrt_tile_bitmap));
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

          current_tile = &tiles[tile_row * tiles_width + tile];
          current_tile->pixels[px_row * NRT_TILE_WIDTH_PX + tile_px] = (char)px;
        }
      }
    }
  }

  printf("Ok, populated pixels!\n");

  // ok, now let's open the rom and clobber our chr bank.
  FILE *outfile = NULL;
  outfile = fopen("mario-test.nes", "r+");

  // read in the header, read in the first chr bank
  // replace the tiles in the chr bank
  // replace the chr bank
  // close the file

  nrt_tile *tile_raw = NRT_TILE_ALLOC;

  nrt_header *rom_header = NRT_HEADER_ALLOC;
  nrt_header_extract(outfile, rom_header);

  nrt_chrbank *chr = NRT_CHR_ALLOC;
  nrt_extract_chr(outfile, rom_header, 0, chr);

  int i;
  for(i = 0; i < tiles_count; i++) {
    nrt_bitmap_to_tile(&tiles[i], tile_raw);
    memcpy(&chr->tile[i], tile_raw, NRT_TILE_SIZE);
  }

  nrt_replace_chr(outfile, rom_header, 0, chr);

  fclose(outfile);
}
