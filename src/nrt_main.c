#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <libgen.h>
#include <getopt.h>

#include "nrt.h"
#include "subcommands.h"
#include "help.h"

int main(int argc, char **argv, char **env) {
  if (argc == 1) {
    print_usage();
    exit(EXIT_SUCCESS);
  }

  char *appname = *(argv++); argc--;
  char *subcommand= *(argv++); argc--;

  if (subcommand[0] == '\0') {
    fprintf(stderr, "Please supply a subcommand.\n");
    print_usage();
    exit(EXIT_FAILURE);
  } else if (is_subcommand(subcommand, "help", "--help", "-h", NULL)) {
    print_usage();
  } else if (is_subcommand(subcommand, "info", NULL)) {
    subcommand_info(argc - 1, argv);
  } else if (is_subcommand(subcommand, "chr", NULL)) {
    subcommand_chr(argc - 1, argv);
  } else if (is_subcommand(subcommand, "prg", NULL)) {
    subcommand_prg(argc - 1, argv);
  } else if (is_subcommand(subcommand, "title", NULL)) {
    subcommand_title(argc - 1, argv);
  } else {
    fprintf(stderr, "Unknown subcommand: %s\n", subcommand);
    print_usage();
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);

  return 0;
}
