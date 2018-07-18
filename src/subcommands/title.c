#include "title.h"

/*
 * usage:
 * nrt title set <rom> [ <options> ] <title>
 * options:
 *   --outfile <file>
 */
title_opts* subcommand_title_set_parse(int argc, char **argv) {
  title_opts *opts = (title_opts*)calloc(1, sizeof(title_opts));

  static struct option longopts[] = {
    { "help", no_argument, NULL, 'h' },
    { "outfile", required_argument, NULL, 'o' },
    { NULL, 0, NULL, 0 }
  };

  if (strcmp(argv[0], "--help") == 0) {
    print_usage_title_set();
    exit(EXIT_SUCCESS);
  }

  strcpy(opts->romfile_path, argv[0]); // don't shift the args

  int ch;

  while ((ch = getopt_long(argc, argv, "o:h", longopts, NULL)) != -1) {
    switch (ch) {
      case 'h':
        print_usage_title_set();
        exit(EXIT_SUCCESS);
        break;

      case 'o':
        strcpy(opts->outfile_path, optarg);

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

  // read in the title from the CLI. make sure that it doesn't go over the max length.

  char *title = argv[0];

  if (strnlen(title, NRT_TITLE_MAX_LENGTH + 1) > NRT_TITLE_MAX_LENGTH) {
    fprintf(stderr, "Title cannot be more than %d characters.\n", NRT_TITLE_MAX_LENGTH);
    free(opts);
    exit(EXIT_FAILURE);
  }

  strncpy(opts->title, title, NRT_TITLE_MAX_LENGTH);

  // if the user didn't specify an outfile, then we want the outfile to be
  // the same as the romfile.
  if (strlen(opts->outfile_path) == 0) {
    strcpy(opts->outfile_path, opts->romfile_path);
  }

  return opts;
}

/*
 * Usage:
 * nrt title get <rom>
 */
title_opts* subcommand_title_get_parse(int argc, char **argv) {
  title_opts *opts = (title_opts*)calloc(1, sizeof(title_opts));

  static struct option longopts[] = {
    { "help", no_argument, NULL, 'h' },
    { NULL, 0, NULL, 0 }
  };

  if (strcmp(argv[0], "--help") == 0) {
    print_usage_title_get();
    exit(EXIT_SUCCESS);
  }

  strcpy(opts->romfile_path, argv[0]); // don't shift the args

  int ch;

  while ((ch = getopt_long(argc, argv, "h", longopts, NULL)) != -1) {
    switch (ch) {
      case 'h':
        print_usage_title_get();
        exit(EXIT_SUCCESS);
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

  return opts;
}

/*
 * usage:
 * nrt title remove <rom> [ <options> ]
 * options:
 *   --outfile <file>
 */
title_opts* subcommand_title_remove_parse(int argc, char **argv) {
  title_opts *opts = (title_opts*)calloc(1, sizeof(title_opts));

  static struct option longopts[] = {
    { "help", no_argument, NULL, 'h' },
    { "outfile", required_argument, NULL, 'o' },
    { NULL, 0, NULL, 0 }
  };

  if (strcmp(argv[0], "--help") == 0) {
    print_usage_title_remove();
    exit(EXIT_SUCCESS);
  }

  strcpy(opts->romfile_path, argv[0]); // don't shift the args

  int ch;

  while ((ch = getopt_long(argc, argv, "o:h", longopts, NULL)) != -1) {
    switch (ch) {
      case 'h':
        print_usage_title_remove();
        exit(EXIT_SUCCESS);
        break;

      case 'o':
        strcpy(opts->outfile_path, optarg);

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

  return opts;
}

void subcommand_title(int argc, char **argv) {
  char *action = *(argv++);

  if (strcmp(action, "set") == 0) {
    SUBCOMMAND(title, set);
  } else if (strcmp(action, "get") == 0) {
    SUBCOMMAND(title, get);
  } else if (strcmp(action, "remove") == 0) {
    SUBCOMMAND(title, remove);
  } else if (strcmp(action, "--help") == 0) {
    print_usage_title();
    exit(EXIT_SUCCESS);
  } else {
    fprintf(stderr, "Invalid action: %s\n", action);
    exit(EXIT_FAILURE);
  }

}

void subcommand_title_set(title_opts *opts) {
  static char tempfile_path[] = "/tmp/nrt-rom.XXXXXX";

  // ok, let's read in the rom file, and replace the chr
  FILE *romfile = fopen(opts->romfile_path, "r");

  if (!romfile) {
    perror("Failed to open rom file for reading");
    exit(EXIT_FAILURE);
  }

  nrt_rom *rom = nrt_read_rom_from_file(romfile);
  fclose(romfile);

  // first we want to zero out the entire title in our in-memory rom
  // then strcpy() the new title in there
  bzero(rom->title, NRT_TITLE_MAX_LENGTH);
  strcpy(rom->title, opts->title);

  int fd = mkstemp(tempfile_path);
  FILE *tempfile = fdopen(fd, "w");

  nrt_rom_write_to_file(rom, tempfile);

  fclose(tempfile);
  nrt_rom_free(rom);

  rename(tempfile_path, opts->outfile_path);
}

void subcommand_title_get(title_opts *opts) {
  FILE *romfile = fopen(opts->romfile_path, "r");

  if (!romfile) {
    perror("Failed to open rom file for reading");
    exit(EXIT_FAILURE);
  }

  nrt_rom *rom = nrt_read_rom_from_file(romfile);
  fclose(romfile);

  if (strnlen(rom->title, NRT_TITLE_MAX_LENGTH) == 0) {
    printf("<no-title>\n");
    nrt_rom_free(rom);
    free(opts);
    exit(EXIT_SUCCESS);
  }

  printf("%s\n", rom->title);
  nrt_rom_free(rom);
  free(opts);

  exit(EXIT_SUCCESS);
}

void subcommand_title_remove(title_opts *opts) {
  // to remove this, we really just need to set the title to a zero-length string.
  bzero(opts->title, NRT_TITLE_MAX_LENGTH);
  subcommand_title_set(opts);
}

