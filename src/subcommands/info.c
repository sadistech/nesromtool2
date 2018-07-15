#include "info.h"

info_opts* subcommand_info_parse(int argc, char **argv) {
  info_opts *opts = (info_opts*)calloc(1, sizeof(info_opts));

  /*
  static struct option longopts[] = {
    { NULL, 0, NULL, 0 }
  };
  */

  strcpy(opts->romfile_path, argv[0]);

  /*
  int ch;

  while ((ch = getopt_long(argc, argv, "", longopts, NULL)) != -1) {
    switch (ch) {
      default:
        fprintf(stderr, "Invalid argument: -%c\n", ch);
        free(opts);
        exit(EXIT_FAILURE);
    }
  }

  argc -= optind;
  argv += optind;
  */

  return opts;
}

void subcommand_info(int argc, char **argv) {
  info_opts *opts = subcommand_info_parse(argc, argv);

  FILE *romfile = fopen(opts->romfile_path, "r");

  if (!romfile) {
    perror("Failed to read in rom file");
    free(opts);
    exit(EXIT_FAILURE);
  }

  nrt_rom *rom = nrt_read_rom_from_file(romfile);
  fclose(romfile);

  if (!nrt_validate_header(rom->header)) {
    printf(
      "Path: %s\n"
      "Valid: FAIL\n",
      opts->romfile_path
    );
    exit(EXIT_FAILURE);
  }

  printf(
    "Path: %s\n"
    "Valid: OK\n"
    "PRG Banks: %d\n"
    "CHR Banks: %d\n"
    "Title: %s\n",
    opts->romfile_path,
    rom->prgs->count,
    rom->chrs->count,
    rom->title
  );

  exit(EXIT_SUCCESS);
}
