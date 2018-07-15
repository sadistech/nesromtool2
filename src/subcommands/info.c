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
    "\n"
    "Mapper lower nybble: %d\n"
    "Ignore mirroring control: %d\n"
    "Trainer present: %d\n"
    "Persistent memory: %d\n"
    "Mirror mode: %d\n"
    "\n"
    "Mapper upper nybble: %d\n"
    "Flag format: %d\n"
    "Playchoice: %d\n"
    "VS Unisystem: %d\n"
    "\n"
    "PRG RAM Size: %d\n"
    "\n"
    "TV System: %d\n"
    "\n"
    "Bus conflicts: %d\n"
    "PRG RAM absent: %d\n"
    "TV System (2): %d\n"
    "\n"
    "Title: %s\n",
    opts->romfile_path,
    rom->prgs->count,
    rom->chrs->count,

    rom->header->mapper_nybble_lower,
    rom->header->ignore_mirroring_control,
    rom->header->trainer_present,
    rom->header->persistent_memory,
    rom->header->mirror_mode,

    rom->header->mapper_nybble_upper,
    rom->header->flag_format,
    rom->header->playchoice,
    rom->header->vs_unisystem,

    rom->header->prg_ram_size,

    rom->header->tv_system,

    rom->header->bus_conflicts,
    rom->header->prg_ram_absent,
    rom->header->tv_system_2,

    rom->title
  );

  exit(EXIT_SUCCESS);
}
