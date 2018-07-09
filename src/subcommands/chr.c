#include "chr.h"

void subcommand_chr(int argc, char **argv) {
  char *action = *(argv++);

  if (strcmp(action, "extract") == 0) {
    subcommand_chr_extract(argc - 1, argv);
  } else if (strcmp(action, "replace") == 0) {
    subcommand_chr_replace(argc - 1, argv);
  }
}

/*
 * Usage:
 * nrt chr extract <file> [ <options> ] <bank> <outfile>
 * options:
 *   --format=[png,raw]
 *   --width=<width-in-tiles>
 */
void subcommand_chr_extract(int argc, char **argv) {
  static struct option longopts[] = {
    { "format", required_argument, NULL, 'f' },
    { "width", required_argument, NULL, 'w' },
    { NULL, 0, NULL, 0 }
  };

  char *infile_path = argv[0]; // don't shift the args
  char *bank_index_str = NULL;

  char format[32] = "";
  int width_in_tiles = 16;

  int bank_index = 0;
  char outfile_path[1024] = "";

  int ch;

  while ((ch = getopt_long(argc, argv, "f:w:", longopts, NULL)) != -1) {
    switch (ch) {
      case 'f':
        strcpy(format, optarg);
        break;
      case 'w':
        width_in_tiles = atoi(optarg);
        break;
      case 0:
        printf("nothing\n");
        break;
      default:
        fprintf(stderr, "Invalid argument: -%c\n", ch);
        exit(EXIT_FAILURE);
    }
  }

  argc -= optind;
  argv += optind;

  // get the remaining args
  bank_index_str = *(argv++);
  strcpy(outfile_path, *(argv++));

  bank_index = atoi(bank_index_str);

  if (strlen(format) == 0) {
    // figure out the format if it's not passed
    const char *ext = filename_ext(outfile_path);

    if (strcmp(ext, "chr") == 0) {
      strcpy(format, "chr");
    } else if (strcmp(ext, "png") == 0) {
      strcpy(format, "png");
    } else {
      fprintf(stderr, "Please supply a format with --format=<fmt>\n");
      exit(EXIT_FAILURE);
    }
  }


  if (width_in_tiles < 1 || width_in_tiles > NRT_CHR_TILE_COUNT) {
    fprintf(stderr, "Width must be between 1 and %d\n", NRT_CHR_TILE_COUNT);
    exit(EXIT_FAILURE);
  }

  printf("Going to open %s and extract bank %d to '%s'\n", infile_path, bank_index, outfile_path);
  printf("Format: '%s'\n", format);

  // now we're ready to write this stuff out.
  FILE *ifile = fopen(infile_path, "r");
  if (!ifile) {
    fprintf(stderr, "Failed to open file: %s\n", infile_path);
    exit(EXIT_FAILURE);
  }

  // read in the header
  // validate header
  // validate that bank exists
  // extract the bank
  // write the output file

  nrt_header *header = NRT_HEADER_ALLOC;
  nrt_header_extract(ifile, header);

  if (! nrt_validate_header(header)) {
    fprintf(stderr, "Rom file does not look valid: %s\n", infile_path);
    fclose(ifile);
    free(header);
    exit(EXIT_FAILURE);
  }

  nrt_chrbank *chr = NRT_CHR_ALLOC;
  if (!nrt_extract_chr(ifile, header, bank_index, chr)) {
    fprintf(stderr, "Failed to read CHR bank %d from %s\n", bank_index, infile_path);
    fclose(ifile);
    free(header);
    free(chr);
    exit(EXIT_FAILURE);
  }

  fclose(ifile);
  free(header);

  FILE *ofile = fopen(outfile_path, "w");

  if (!ofile) {
    perror("Failed to open file for writing");
    exit(EXIT_FAILURE);
  }

  if (strcmp(format, "chr") == 0) {
    // just write out the data to the file.

    fwrite(chr, NRT_CHR_BANK_SIZE, 1, ofile);

    fclose(ofile);
    free(chr);
  } else if (strcmp(format, "png") == 0) {
    // write out the PNG
    // first, create bitmaps, then write.

    nrt_tile_bitmap *bitmaps = (nrt_tile_bitmap*)malloc(NRT_CHR_TILE_COUNT * sizeof(nrt_tile_bitmap));
    int i;
    for (i = 0; i < NRT_CHR_TILE_COUNT; i++) {
      nrt_tile_to_bitmap(&chr->tile[i], &bitmaps[i]);
    }

    nrt_tiles_to_png(bitmaps, NRT_CHR_TILE_COUNT, width_in_tiles, ofile);
  } else {
    fprintf(stderr, "Invalid format: %s\n", format);
    free(chr);
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

/*
 * replace a CHR bank from a raw file or a png.
 * usage:
 * nrt chr replace <rom> [ <options> ] <bank-index> <bank-file>
 * options:
 *  --format=<png|chr>
 *  --outfile=<outfilepath>
 */
void subcommand_chr_replace(int argc, char **argv) {
  static struct option longopts[] = {
    { "format", required_argument, NULL, 'f' },
    { "outfile", required_argument, NULL, 'o' },
    { NULL, 0, NULL, 0 }
  };

  char *infile_path = argv[0]; // don't shift the args
  char *bank_index_str = NULL;

  char format[32] = "";
  int bank_index = 0;
  char outfile_path[1024] = "";
  char bankfile_path[1024] = "";
  static char tempfile_path[] = "/tmp/nrt-rom.XXXXXX";

  int ch;

  while ((ch = getopt_long(argc, argv, "f:o:", longopts, NULL)) != -1) {
    switch (ch) {
      case 'f':
        strcpy(format, optarg);
        break;
      case 'o':
        strcpy(outfile_path, optarg);
        break;
      case 0:
        printf("nothing\n");
        break;
      default:
        fprintf(stderr, "Invalid argument: -%c\n", ch);
        exit(EXIT_FAILURE);
    }
  }

  argc -= optind;
  argv += optind;

  bank_index_str = *(argv++);
  strcpy(bankfile_path, *(argv++));

  bank_index = atoi(bank_index_str);

  if (strlen(format) == 0) {
    // figure out the format if it's not passed
    const char *ext = filename_ext(bankfile_path);

    if (strcmp(ext, "chr") == 0) {
      strcpy(format, "chr");
    } else if (strcmp(ext, "png") == 0) {
      strcpy(format, "png");
    } else {
      fprintf(stderr, "Please supply a format with --format=<fmt>\n");
      exit(EXIT_FAILURE);
    }
  }

  if (strlen(outfile_path) == 0) {
    // clobber the existing ROM
    strcpy(outfile_path, infile_path);
  }

  // now we want to read in the source file, and get the data we really want
  // then we'll read in the rom, replace the bank and write it to a tempfile
  // then move the tempfile to the final location

  nrt_chrbank *chr_bank = NRT_CHR_ALLOC;

  if (strcmp(format, "png") == 0) {
    FILE *pngfile = fopen(bankfile_path, "r");
    if (!pngfile) {
      perror("Failed to open up bank file");
      exit(EXIT_FAILURE);
    }

    nrt_tile_bitmap *tiles = (nrt_tile_bitmap*)malloc(NRT_CHR_TILE_COUNT * sizeof(nrt_tile_bitmap));

    if (! nrt_png_to_tiles(pngfile, tiles)) {
      fprintf(stderr, "Failed to read in PNG file: %s\n", bankfile_path);
      fclose(pngfile);
      exit(EXIT_FAILURE);
    }

    // convert from bitmap to tiles in bank.
    nrt_tile *raw_tile = NRT_TILE_ALLOC;
    int i;
    for(i = 0; i < NRT_CHR_TILE_COUNT; i++) {
      nrt_bitmap_to_tile(&tiles[i], raw_tile);
      memcpy(&chr_bank->tile[i], raw_tile, NRT_TILE_SIZE);
    }

    free(raw_tile);
    free(tiles);
  } else if (strcmp(format, "chr") == 0) {
    printf("Not implemented\n");
    exit(EXIT_FAILURE);
  }

  // ok, let's read in the rom file, and replace the chr
  FILE *romfile = fopen(infile_path, "r");

  if (!romfile) {
    perror("Failed to open rom file for reading");
    exit(EXIT_FAILURE);
  }

  nrt_rom *rom = nrt_read_rom_from_file(romfile);
  fclose(romfile);

  nrt_chr_listitem *bank_listitem = nrt_chr_listitem_at(rom->chrs, bank_index);
  free(bank_listitem->chr);
  bank_listitem->chr = chr_bank;

  int fd = mkstemp(tempfile_path);
  FILE *tempfile = fdopen(fd, "w");

  nrt_rom_write_to_file(rom, tempfile);

  fclose(tempfile);
  nrt_rom_free(rom);

  rename(tempfile_path, outfile_path);

}
