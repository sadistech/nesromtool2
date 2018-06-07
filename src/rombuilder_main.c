#include <stdio.h>
#include <unistd.h>
#include "nrt.h"

// usage:
// ./rombuilder <romfile> 2 3 "hello world"
// creates a ROM file with 2 PRG banks, 3 CHR banks and a title of hello world

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("Not enough args.\n");
    exit(EXIT_FAILURE);
  }

  char *rompath = argv[1];
  char *in_prg_count = argv[2];
  char *in_chr_count = argv[3];
  char *in_title = argv[4];

  int prg_count = atoi(in_prg_count);
  int chr_count = atoi(in_chr_count);

  printf("Will create a new ROM @ %s:\n", rompath);
  printf("  %d PRG banks.\n  %d CHR banks.\n", prg_count, chr_count);
  printf("  Title: %s\n", in_title);

  nrt_header* header = nrt_header_create(prg_count, chr_count);
  nrt_prgbank* prg = NRT_PRG_ALLOC;
  nrt_chrbank* chr = NRT_CHR_ALLOC;
  char title[NRT_TITLE_MAX_LENGTH];

  if (in_title) {
    strcpy(title, in_title);
  }

  bzero(prg->data, NRT_PRG_BANK_SIZE);
  bzero(chr->tile, NRT_CHR_BANK_SIZE);

  // open the file

  FILE* rom = NULL;

  if (!(rom = fopen(rompath, "w+"))) {
    printf("Failed to write the file.\n");
    exit(EXIT_FAILURE);
  }

  // write the header
  if (fwrite(header, NRT_HEADER_SIZE, 1, rom) != 1) {
    printf("Failed to write the header.\n");
    exit(EXIT_FAILURE);
  }

  // write the banks
  char memo[10];

  int i;
  for (i = 0; i < prg_count; i++ ) {
    sprintf(memo, "PRG %02d", i);
    memcpy(prg->data, memo, strlen(memo) + 1);

    if (fwrite(prg, NRT_PRG_BANK_SIZE, 1, rom) != 1) {
      printf("Failed to write PRG %d\n", i);
      exit(EXIT_FAILURE);
    }
  }

  for (i = 0; i < chr_count; i++ ) {
    sprintf(memo, "CHR %02d", i);
    memcpy(prg->data, memo, strlen(memo) + 1);

    if (fwrite(prg, NRT_CHR_BANK_SIZE, 1, rom) != 1) {
      printf("Failed to write CHR %d\n", i);
      exit(EXIT_FAILURE);
    }

  }

  // write the title
  if (in_title) {
    if (fwrite(title, NRT_TITLE_MAX_LENGTH, 1, rom) != 1) {
      printf("Failed to write title.\n");
      exit(EXIT_FAILURE);
    }
  }

  fclose(rom);

  printf("Done!\n");
  exit(EXIT_SUCCESS);
}
