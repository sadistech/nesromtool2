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
    { "outfile", required_argument, NULL, 'o' },
    { NULL, 0, NULL, 0 }
  };

  strcpy(opts->romfile_path, argv[0]); // don't shift the args

  int ch;

  while ((ch = getopt_long(argc, argv, "o:", longopts, NULL)) != -1) {
    switch (ch) {
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

  return opts;
}

/*
 * Usage:
 * nrt title get <rom>
 */
title_opts* subcommand_title_get_parse(int argc, char **argv) {
  title_opts *opts = (title_opts*)calloc(1, sizeof(title_opts));

  static struct option longopts[] = {
    { NULL, 0, NULL, 0 }
  };

  strcpy(opts->romfile_path, argv[0]); // don't shift the args

  int ch;

  while ((ch = getopt_long(argc, argv, "", longopts, NULL)) != -1) {
    switch (ch) {
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
    { "outfile", required_argument, NULL, 'o' },
    { NULL, 0, NULL, 0 }
  };

  strcpy(opts->romfile_path, argv[0]); // don't shift the args

  int ch;

  while ((ch = getopt_long(argc, argv, "o:", longopts, NULL)) != -1) {
    switch (ch) {
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
  }
}

void subcommand_title_set(title_opts *opts) {
  printf("Going to set title on %s to: %s\n", opts->romfile_path, opts->title);
  exit(EXIT_SUCCESS);
}

void subcommand_title_get(title_opts *opts) {
  printf("Going to get title on %s\n", opts->romfile_path);
  exit(EXIT_SUCCESS);
}

void subcommand_title_remove(title_opts *opts) {
  printf("Going to remove title from %s\n", opts->romfile_path);
  exit(EXIT_SUCCESS);
}

