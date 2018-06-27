#include "nrt.h"
#include <stdio.h>
#include <png.h>

int main() {
  FILE *infile = NULL;
  FILE *outfile = NULL;

  char *filename = "./mario.nes";

  infile = fopen(filename, "r");

  // first lets get the first tile in here.
  nrt_chrbank *chr = NRT_CHR_ALLOC;
  nrt_header *header = NRT_HEADER_ALLOC;
  nrt_header_extract(infile, header);
  nrt_extract_chr(infile, header, 0, chr);

  fclose(infile);

  nrt_tile *tile = &chr->tile[1];
  nrt_tile_bitmap *bitmap = NRT_TILE_BITMAP_ALLOC;
  nrt_tile_to_bitmap(tile, bitmap);

  outfile = fopen("sprite.png", "wb");

  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info_ptr = png_create_info_struct(png_ptr);

  if (!info_ptr) {
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    exit(EXIT_FAILURE);
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(outfile);
    exit(EXIT_FAILURE);
  }

  // set up the palette.
  static const png_color palette[] = {
    {0x00, 0x00, 0x00},
    {0xFF, 0x00, 0x00},
    {0xFF, 0xFF, 0x00},
    {0x00, 0x00, 0xFF}
  };

  png_byte *row = (png_byte*)malloc(2 * sizeof(png_byte));

  png_init_io(png_ptr, outfile);
//png_set_write_status_fn(png_ptr, write_row_callback);

  png_set_IHDR(png_ptr, info_ptr, 8, 8, 2, PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  png_set_PLTE(png_ptr, info_ptr, palette, 4);

  png_write_info(png_ptr, info_ptr);

  int i;
  int j;
  int idx = 0;
  for (i = 0; i < 8; i++) {
    // now that we're on the row, we have to write the binary index color to the row
    row[0] = 0;
    row[1] = 0;

    for (j = 0; j < 8; j++) {
      if (j <= 3) {
        idx = 0;
        // first byte
      } else {
        idx = 1;
        //second byte
      }
      row[idx] = row[idx] << 2;
      row[idx] += bitmap->pixels[i * 8 + j];
    }

    png_write_row(png_ptr, row);
  }

  png_write_end(png_ptr, NULL);

  if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
  free(row);

  fclose(outfile);

  printf("hello\n");
}
