#include "chr.h"

chr_output_format parse_chr_output_format(char *format) {
  if (strcmp(format, "raw") == 0) {
    return raw_format;
  } else if (strcmp(format, "png") == 0) {
    return png_format;
  }

  return null_format;
}

chr_output_format filename_to_chr_output_format(char *filename) {
  // figure out the format if it's not passed
  const char *ext = filename_ext(filename);

  if (strcmp(ext, "chr") == 0) {
    return raw_format;
  } else if (strcmp(ext, "png") == 0) {
    return png_format;
  }

  return null_format;
}

/*
 * Usage:
 * nrt chr extract <file> [ <options> ] <bank> <outfile>
 * options:
 *   --format=[png,raw]
 *   --width=<width-in-tiles>
 */
chr_opts* subcommand_chr_extract_parse(int argc, char **argv) {
  chr_opts *opts = (chr_opts*)calloc(1, sizeof(chr_opts));

  // default settings
  opts->width = 16;

  static struct option longopts[] = {
    { "help", no_argument, NULL, 'h' },
    { "format", required_argument, NULL, 'f' },
    { "width", required_argument, NULL, 'w' },
    { NULL, 0, NULL, 0 }
  };

  if (strcmp(argv[0], "--help") == 0) {
    print_usage_chr_extract();
    exit(EXIT_SUCCESS);
  }

  strcpy(opts->romfile_path, argv[0]); // don't shift the args

  int ch;

  while ((ch = getopt_long(argc, argv, "f:w:h", longopts, NULL)) != -1) {
    switch (ch) {
      case 'h':
        print_usage_chr_extract();
        exit(EXIT_SUCCESS);
        break;

      case 'f':
        opts->format = parse_chr_output_format(optarg);

        if (opts->format == null_format) {
          // the format was invalid
          fprintf(stderr, "Invalid format: %s\n", optarg);
          free(opts);
          exit(EXIT_FAILURE);
        }

        break;
      case 'w':
        opts->width = atoi(optarg);
        break;
      case 0:
        break;
      default:
        fprintf(stderr, "Invalid argument: -%c\n", ch);
        exit(EXIT_FAILURE);
    }
  }

  argc -= optind;
  argv += optind;

  // read the bank index
  char *bank_index_str = *(argv++);
  opts->bank_index = atoi(bank_index_str);

  // read the outfile path
  strcpy(opts->outfile_path, *(argv++));

  // try to guess the format if the user didn't supply one
  if (! opts->format) {
    opts->format = filename_to_chr_output_format(opts->outfile_path);
  }

  if (! opts->format) {
    fprintf(stderr, "Could not glean the format from the output file name.");
    free(opts);
    exit(EXIT_FAILURE);
  }

  if (opts->width < 1 || opts->width > NRT_CHR_TILE_COUNT) {
    fprintf(stderr, "Width must be between 1 and %d\n", NRT_CHR_TILE_COUNT);
    free(opts);
    exit(EXIT_FAILURE);
  }

  /* printf("Going to open %s and extract bank %d to '%s'\n", opts->romfile_path, opts->bank_index, opts->outfile_path); */
  /* printf("Format: '%d'\n", opts->format); */

  return opts;
}

chr_opts* subcommand_chr_replace_parse(int argc, char **argv) {
  chr_opts *opts = (chr_opts*)calloc(1, sizeof(chr_opts));

  static struct option longopts[] = {
    { "help", no_argument, NULL, 'h' },
    { "format", required_argument, NULL, 'f' },
    { "outfile", required_argument, NULL, 'o' },
    { NULL, 0, NULL, 0 }
  };

  if (strcmp(argv[0], "--help") == 0) {
    print_usage_chr_replace();
    exit(EXIT_SUCCESS);
  }

  strcpy(opts->romfile_path, argv[0]);

  int ch;

  while ((ch = getopt_long(argc, argv, "f:o:", longopts, NULL)) != -1) {
    switch (ch) {
      case 'h':
        print_usage_chr_replace();
        exit(EXIT_SUCCESS);
        break;

      case 'f':
        opts->format = parse_chr_output_format(optarg);

        if (opts->format == null_format) {
          // the format was invalid
          fprintf(stderr, "Invalid format: %s\n", optarg);
          free(opts);
          exit(EXIT_FAILURE);
        }
        break;
      case 'o':
        strcpy(opts->sourcefile_path, optarg);
        break;
      default:
        fprintf(stderr, "Invalid argument: -%c\n", ch);
        free(opts);
        exit(EXIT_FAILURE);
    }
  }

  argc -= optind;
  argv += optind;

  // read the bank index
  char *bank_index_str = *(argv++);
  opts->bank_index = atoi(bank_index_str);

  strcpy(opts->sourcefile_path, *(argv++));

  // try to guess the format if the user didn't supply one
  if (! opts->format) {
    opts->format = filename_to_chr_output_format(opts->outfile_path);
  }

  if (! opts->format) {
    fprintf(stderr, "Could not glean the format from the output file name.");
    free(opts);
    exit(EXIT_FAILURE);
  }

  if (strlen(opts->outfile_path) == 0) {
    // clobber the existing ROM
    strcpy(opts->outfile_path, opts->romfile_path);
  }

  return opts;
}

void subcommand_chr(int argc, char **argv) {
  char *action = *(argv++);

  if (strcmp(action, "extract") == 0) {
    SUBCOMMAND(chr, extract);
  } else if (strcmp(action, "replace") == 0) {
    SUBCOMMAND(chr, replace);
  } else if (strcmp(action, "--help") == 0) {
    print_usage_chr();
    exit(EXIT_SUCCESS);
  } else {
    fprintf(stderr, "Invalid action: %s\n", action);
    exit(EXIT_FAILURE);
  }
}

void subcommand_chr_extract(chr_opts *opts) {
  // now we're ready to write this stuff out.
  FILE *ifile = fopen(opts->romfile_path, "r");
  if (!ifile) {
    fprintf(stderr, "Failed to open file: %s\n", opts->romfile_path);
    free(opts);
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
    fprintf(stderr, "Rom file does not look valid: %s\n", opts->romfile_path);
    fclose(ifile);
    free(header);
    free(opts);
    exit(EXIT_FAILURE);
  }

  nrt_chrbank *chr = NRT_CHR_ALLOC;
  if (!nrt_extract_chr(ifile, header, opts->bank_index, chr)) {
    fprintf(stderr, "Failed to read CHR bank %d from %s\n", opts->bank_index, opts->romfile_path);
    fclose(ifile);
    free(header);
    free(chr);
    free(opts);
    exit(EXIT_FAILURE);
  }

  fclose(ifile);
  free(header);

  FILE *ofile = fopen(opts->outfile_path, "w");

  if (!ofile) {
    perror("Failed to open file for writing");
    exit(EXIT_FAILURE);
  }

  if (opts->format == raw_format) {
    // just write out the data to the file.

    fwrite(chr, NRT_CHR_BANK_SIZE, 1, ofile);

    fclose(ofile);
    free(chr);
  } else if (opts->format == png_format) {
    // write out the PNG
    // first, create bitmaps, then write.

    nrt_tile_bitmap *bitmaps = (nrt_tile_bitmap*)malloc(NRT_CHR_TILE_COUNT * sizeof(nrt_tile_bitmap));
    int i;
    for (i = 0; i < NRT_CHR_TILE_COUNT; i++) {
      nrt_tile_to_bitmap(&chr->tile[i], &bitmaps[i]);
    }

    nrt_tiles_to_png(bitmaps, NRT_CHR_TILE_COUNT, opts->width, ofile);
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
void subcommand_chr_replace(chr_opts *opts) {
  // now we want to read in the source file, and get the data we really want
  // then we'll read in the rom, replace the bank and write it to a tempfile
  // then move the tempfile to the final location

  nrt_chrbank *chr_bank = NRT_CHR_ALLOC;

  if (opts->format == png_format) {
    FILE *pngfile = fopen(opts->sourcefile_path, "r");
    if (!pngfile) {
      perror("Failed to open up bank file");
      free(opts);
      exit(EXIT_FAILURE);
    }

    nrt_tile_bitmap_lockup *lockup= (nrt_tile_bitmap_lockup*)calloc(NRT_CHR_TILE_COUNT, sizeof(nrt_tile_bitmap_lockup));

    if (! nrt_png_to_tiles(pngfile, lockup)) {
      fprintf(stderr, "Failed to read in PNG file: %s\n", opts->sourcefile_path);
      fclose(pngfile);
      free(opts);
      exit(EXIT_FAILURE);
    }

    // convert from bitmap to tiles in bank.
    nrt_tile *raw_tile = NRT_TILE_ALLOC;
    int i;
    for(i = 0; i < NRT_CHR_TILE_COUNT; i++) {
      nrt_bitmap_to_tile(&lockup->bitmaps[i], raw_tile);
      memcpy(&chr_bank->tile[i], raw_tile, NRT_TILE_SIZE);
    }

    free(raw_tile);
    free(lockup->bitmaps);
    free(lockup);
  } else if (opts->format == raw_format) {
    printf("Not implemented\n");
    exit(EXIT_FAILURE);
  }

  static char tempfile_path[] = "/tmp/nrt-rom.XXXXXX";

  // ok, let's read in the rom file, and replace the chr
  FILE *romfile = fopen(opts->outfile_path, "r");

  if (!romfile) {
    perror("Failed to open rom file for reading");
    exit(EXIT_FAILURE);
  }

  nrt_rom *rom = nrt_read_rom_from_file(romfile);
  fclose(romfile);

  nrt_chr_listitem *bank_listitem = nrt_chr_listitem_at(rom->chrs, opts->bank_index);
  free(bank_listitem->chr);
  bank_listitem->chr = chr_bank;

  int fd = mkstemp(tempfile_path);
  FILE *tempfile = fdopen(fd, "w");

  nrt_rom_write_to_file(rom, tempfile);

  fclose(tempfile);
  nrt_rom_free(rom);

  rename(tempfile_path, opts->outfile_path);
}
