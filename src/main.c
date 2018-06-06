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

  nrt_error *error = NRT_ERROR_ALLOC;

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

  nrt_header *header = NRT_HEADER_ALLOC;

  if (fread(header, NRT_HEADER_SIZE, 1, rom) != 1) {
    nrt_die("Failed reading the header of the file.");
  }

  if (strcmp(type, "prg") == 0) {
    nrt_prgbank *prg = NRT_PRG_ALLOC;

    if (!nrt_prg_index_valid(header, index)) {
      nrt_die("Invalid PRG bank index: %d (There are %d PRG banks)", index, header->prg_count);
    }

    if (nrt_extract_prg(rom, index, prg) != 1) {
      nrt_die("Failed to read PRG bank");
    }

    if (!(outfile = fopen(outpath, "w+"))) {
      nrt_die("Failed to open PRG outfile.");
    }

    if (fwrite(prg, NRT_PRG_BANK_SIZE, 1, outfile) != 1) {
      nrt_die("Failed to write PRG data to file.");
    }

    free(prg);
  } else if (strcmp(type, "chr") == 0) {
    nrt_chrbank *chr = NRT_CHR_ALLOC;

    if (!nrt_prg_index_valid(header, index)) {
      nrt_die("Invalid CHR bank index: %d (There are %d CHR banks)", index, header->chr_count);
    }

    if (nrt_extract_chr(rom, header, index, chr) != 1) {
      nrt_die("Failed to read CHR bank.");
    }

    // ok, now write the file.
    if (!(outfile = fopen(outpath, "w+"))) {
      nrt_die("Failed to open CHR outfile.");
    }

    if (fwrite(chr, NRT_CHR_BANK_SIZE, 1, outfile) != 1) {
      nrt_die("Failed to write CHR data to file.");
    }

    free(chr);
  }

  free(header);
  fclose(rom);
  fclose(outfile);
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
