#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <libgen.h>

#include "nrt.h"

// verify <path>
void action_verify(char **argv) {
  char *filepath = argv[0];
  FILE *ifile = NULL;

  if (!(ifile = fopen(filepath, "r+"))) {
    fprintf(stderr, "Failed to open file.\n");
    exit(EXIT_FAILURE);
  }

  nrt_error_t *error = NRT_ERROR_ALLOC;

  nrt_validate_file(ifile, error);

  if (nrt_is_error(error)) {
    printf("%s: FAIL\n", basename(filepath));
    exit(EXIT_FAILURE);
  } else {
    printf("%s: OK\n", basename(filepath));
    exit(EXIT_SUCCESS);
  }
}

// nrt extract chr <index> <rom> <outfile>
void action_extract(char **argv) {
  char *type = *(argv++);
  char *text_index = *(argv++);
  char *rompath = *(argv++);
  char *outpath = *(argv++);

  int index = atoi(text_index);

  FILE *rom = NULL;
  FILE *outfile = NULL;

  printf("Going to extract a %s from %s to %s\n", type, rompath, outpath);

  if (!(rom = fopen(rompath, "r+"))) {
    nrt_die("Failed to open file.");
  }

  nrt_chrbank_t *chr = NRT_CHR_ALLOC;

  if (nrt_extract_chr(rom, index, chr) != 1) {
    nrt_die("Failed to read CHR bank.");
  }

  // ok, now write the file.
  if (!(outfile = fopen(outpath, "w+"))) {
    nrt_die("Failed to open CHR outfile.");
  }

  if (fwrite(chr, NRT_CHR_BANK_SIZE, 1, outfile) != 1) {
    nrt_die("Failed to write CHR data to file.");
  }

  fclose(rom);
  fclose(outfile);
  free(chr);
}

int main(int argc, char **argv, char **env) {
  char *appname = *(argv++);
  char *action = *(argv++);

  if (strcmp(action, "verify") == 0) {
    action_verify(argv);
  } else if ( strcmp(action, "extract") == 0 ) {
    action_extract(argv);
  } else {
    perror("No action.");
    exit(EXIT_FAILURE);
  }

  return 0;
}
