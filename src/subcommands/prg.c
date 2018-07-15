#include "prg.h"

prg_output_format parse_prg_output_format(char *format) {
  if (strcmp(format, "raw") == 0) {
    return prg_raw_format;
  }

  return prg_null_format;
}

prg_output_format filename_to_prg_output_format(char *filename) {
  // figure out the format if it's not passed
  const char *ext = filename_ext(filename);

  if (strcmp(ext, "prg") == 0) {
    return prg_raw_format;
  }

  return prg_null_format;
}

/*
 * Usage:
 * nrt prg extract <file> [ <options> ] <bank> <outfile>
 * options:
 *   --format=[raw]
 */
prg_opts* subcommand_prg_extract_parse(int argc, char **argv) {
  prg_opts *opts = (prg_opts*)calloc(1, sizeof(prg_opts));

  static struct option longopts[] = {
    { "format", required_argument, NULL, 'f' },
    { NULL, 0, NULL, 0 }
  };

  strcpy(opts->romfile_path, argv[0]); // don't shift the args

  int ch;

  while ((ch = getopt_long(argc, argv, "f:", longopts, NULL)) != -1) {
    switch (ch) {
      case 'f':
        opts->format = parse_prg_output_format(optarg);

        if (opts->format == prg_null_format) {
          // the format was invalid
          fprintf(stderr, "Invalid format: %s\n", optarg);
          free(opts);
          exit(EXIT_FAILURE);
        }

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
    opts->format = filename_to_prg_output_format(opts->outfile_path);
  }

  if (! opts->format) {
    fprintf(stderr, "Could not glean the format from the output file name.");
    free(opts);
    exit(EXIT_FAILURE);
  }

  return opts;
}

/*
 * Usage:
 * nrt prg replace <file> [ <options> ] <bank> <sourceprg>
 * options:
 *   --format=[raw]
 *   --outfile=<path>
 */
prg_opts* subcommand_prg_replace_parse(int argc, char **argv) {
  prg_opts *opts = (prg_opts*)calloc(1, sizeof(prg_opts));

  static struct option longopts[] = {
    { "format", required_argument, NULL, 'f' },
    { "outfile", required_argument, NULL, 'o' },
    { NULL, 0, NULL, 0 }
  };

  strcpy(opts->romfile_path, argv[0]);

  int ch;

  while ((ch = getopt_long(argc, argv, "f:o:", longopts, NULL)) != -1) {
    switch (ch) {
      case 'f':
        opts->format = parse_prg_output_format(optarg);

        if (opts->format == prg_null_format) {
          // the format was invalid
          fprintf(stderr, "Invalid format: %s\n", optarg);
          free(opts);
          exit(EXIT_FAILURE);
        }
        break;
      case 'o':
        strcpy(opts->outfile_path, optarg);
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
    opts->format = filename_to_prg_output_format(opts->sourcefile_path);
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

void subcommand_prg(int argc, char **argv) {
  char *action = *(argv++);

  if (strcmp(action, "extract") == 0) {
    SUBCOMMAND(prg, extract);
  } else if (strcmp(action, "replace") == 0) {
    SUBCOMMAND(prg, replace);
  } else {
    fprintf(stderr, "Invalid action: %s\n", action);
    exit(EXIT_FAILURE);
  }
}

void subcommand_prg_extract(prg_opts *opts) {
  // now we're ready to write this stuff out.
  FILE *ifile = fopen(opts->romfile_path, "r");
  if (!ifile) {
    fprintf(stderr, "Failed to open file: %s\n", opts->romfile_path);
    exit(EXIT_FAILURE);
  }

  nrt_header *header = NRT_HEADER_ALLOC;
  nrt_header_extract(ifile, header);

  if (! nrt_validate_header(header)) {
    fprintf(stderr, "Rom file does not look valid: %s\n", opts->romfile_path);
    fclose(ifile);
    free(header);
    free(opts);
    exit(EXIT_FAILURE);
  }

  free(header);

  nrt_prgbank *prg = NRT_PRG_ALLOC;
  if (!nrt_extract_prg(ifile, opts->bank_index, prg)) {
    fprintf(stderr, "Failed to read PRG bank %d from %s\n", opts->bank_index, opts->romfile_path);
    fclose(ifile);
    free(prg);
    free(opts);
    exit(EXIT_FAILURE);
  }

  fclose(ifile);

  // ok, now let's write out the PRG
  FILE *ofile = fopen(opts->outfile_path, "w");

  if (!ofile) {
    perror("Failed to open file for writing");
    exit(EXIT_FAILURE);
  }

  if (opts->format == prg_raw_format) {
    // just write out the data to the file.

    fwrite(prg, NRT_PRG_BANK_SIZE, 1, ofile);

    fclose(ofile);
    free(prg);
  }

  free(opts);
  exit(EXIT_SUCCESS);
}

void subcommand_prg_replace(prg_opts *opts) {
  nrt_prgbank *prg_bank = NRT_PRG_ALLOC;

  if (opts->format == prg_raw_format) {
    // it's a raw file, so let's read that in.
    FILE *ifile = fopen(opts->sourcefile_path, "r");
    if (!ifile) {
      perror("Failed to open source file");
      free(prg_bank);
      free(opts);
      exit(EXIT_FAILURE);
    }

    // read in the data.
    if (fread(prg_bank, NRT_PRG_BANK_SIZE, 1, ifile) != 1) {
      perror("Failed to read prg bank from file");
      fclose(ifile);
      free(prg_bank);
      free(opts);
      exit(EXIT_FAILURE);
    }

    fclose(ifile);
  } else {
    fprintf(stderr, "Unsupported format: %d\n", opts->format);
  }

  static char tempfile_path[] = "/tmp/nrt-rom.XXXXXX";

  // ok, let's read in the rom file, and replace the chr
  FILE *romfile = fopen(opts->romfile_path, "r");

  if (!romfile) {
    perror("Failed to open rom file for reading");
    exit(EXIT_FAILURE);
  }

  nrt_rom *rom = nrt_read_rom_from_file(romfile);
  fclose(romfile);

  nrt_prg_listitem *bank_listitem = nrt_prg_listitem_at(rom->prgs, opts->bank_index);
  free(bank_listitem->prg);
  bank_listitem->prg = prg_bank;

  int fd = mkstemp(tempfile_path);
  FILE *tempfile = fdopen(fd, "w");

  nrt_rom_write_to_file(rom, tempfile);

  fclose(tempfile);
  nrt_rom_free(rom);

  rename(tempfile_path, opts->outfile_path);
}

