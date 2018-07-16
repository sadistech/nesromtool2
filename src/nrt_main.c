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

int main(int argc, char **argv, char **env) {
  char *appname = *(argv++); argc--;
  char *subcommand= *(argv++); argc--;

  if (strcmp(subcommand, "") == 0 ) {
    fprintf(stderr, "Please supply a subcommand.\n");
    exit(EXIT_FAILURE);
  } else if (strcmp(subcommand, "info") == 0) {
    subcommand_info(argc - 1, argv);
  } else if (strcmp(subcommand, "chr") == 0) {
    subcommand_chr(argc - 1, argv);
  } else if (strcmp(subcommand, "prg") == 0) {
    subcommand_prg(argc - 1, argv);
  } else if (strcmp(subcommand, "title") == 0) {
    subcommand_title(argc - 1, argv);
  } else {
    fprintf(stderr, "Unknown subcommand: %s\n", subcommand);
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);

  return 0;
}
