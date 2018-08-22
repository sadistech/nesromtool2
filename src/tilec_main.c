#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <libgen.h>

#include "nrt.h"
#include "util.h"

int last_offset; // if is_in_bank() returns true, this is set to the offset of that tile

bool is_in_bank(nrt_chrbank *bank, int bank_size, nrt_tile *tile);

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <outfile> <infile> [ <infile> ... ]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *appname = *(argv++); argc--;
  char *outfile_path = *(argv++); argc--;

  char *infile_path = NULL;
  FILE *ifile = NULL;

  FILE *nametable = fopen("nametable.inc", "w");
  FILE *vars = fopen("vars.inc", "w");

  int i;

  nrt_chrbank *chr = NRT_CHR_ALLOC;
  int bank_cursor = 0;

  // the tile that we're currently working with
  nrt_tile *tile = NRT_TILE_ALLOC;

  nrt_tile_bitmap *bitmaps = (nrt_tile_bitmap*)calloc(NRT_CHR_TILE_COUNT, sizeof(nrt_tile_bitmap));

  printf("Will write %s\n", outfile_path);

  while ((infile_path = *(argv++))) {
    printf("Processing: %s\n", infile_path);
    char *varname = filename_no_ext(basename(infile_path));

    // convert file into tiles

    // first, open file for reading
    if (!(ifile = fopen(infile_path, "r"))) {
      perror(infile_path);
      exit(EXIT_FAILURE);
    }

    // start converting
    nrt_tile_bitmap_lockup *lockup = (nrt_tile_bitmap_lockup*)calloc(1, sizeof(nrt_tile_bitmap_lockup));

    // read the bitmap
    if (! nrt_png_to_tiles(ifile, lockup)) {
      fprintf(stderr, "Something went wrong with reading the png.\n");
      fclose(ifile);
      exit(EXIT_FAILURE);
    }

    printf("Read lockup: %d tiles and width: %d\n", lockup->count, lockup->width);

    // now let's move them to the bank
    for (i = 0; i < lockup->count; i++) {
      nrt_bitmap_to_tile(&lockup->bitmaps[i], tile);

      if (is_in_bank(chr, bank_cursor, tile)) {
        fprintf(vars, "%s_%i = $%02x\n", varname, i, last_offset);
        fprintf(stderr, "Tile %d exists, skipping...\n", i);
        continue;
      }

      fprintf(stderr, "Tile %d does not exist. adding!\n", i);

      fprintf(vars, "%s_%i = $%02x\n", varname, i, bank_cursor);
      // it was not in the bank, let's append.
      memcpy(&(chr->tile[bank_cursor]), tile, NRT_TILE_SIZE);
      bank_cursor++;

      if (bank_cursor > NRT_CHR_TILE_COUNT) {
        fprintf(stderr, "TOO MANY SPRITES (%d)\n", bank_cursor);
        exit(EXIT_FAILURE);
      }
    }

    free(lockup->bitmaps);
    free(lockup);
  }

  // convert back into bitmaps
  for (i = 0; i < NRT_CHR_TILE_COUNT; i++) {
    nrt_tile_to_bitmap(&chr->tile[i], &bitmaps[i]);
  }

  free(chr);

  // now let's write that file.

  FILE *ofile = fopen(outfile_path, "w");

  if (!ofile) {
    perror(outfile_path);
    exit(EXIT_FAILURE);
  }

  // write to file
  nrt_tiles_to_png(bitmaps, NRT_CHR_TILE_COUNT, 16, ofile);

  fclose(ofile);

  printf("Done.\n");
}

bool is_in_bank(nrt_chrbank *bank, int bank_size, nrt_tile *tile) {
  int i;
  for (i = 0; i < bank_size; i++) {
    if (memcmp(&(bank->tile[i]), tile, NRT_TILE_SIZE) == 0) {
      // they match!
      last_offset = i;
      return true;
    }
  }

  return false;
}
