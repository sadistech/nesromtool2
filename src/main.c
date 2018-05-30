#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <libgen.h>

#include "nesromtool.h"

// verify <path>
void action_verify(char *filepath) {
  FILE *ifile = NULL;

  if (!(ifile = fopen(filepath, "r+"))) {
    fprintf(stderr, "Failed to open file.");
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

int main(int argc, char **argv, char **env) {
  char *appname = *(argv++);
  char *action = *(argv++);
  char *filepath = *(argv++);

  if (strcmp(action, "verify") == 0) {
    action_verify(filepath);
  } else {
    perror("No action.");
    exit(EXIT_FAILURE);
  }


  /* printf("Gonna [%s] open %s\n", action, filepath); */


  /* assert(strncmp(header->magic_word, NES_MAGIC_WORD, NES_MAGIC_WORD_SIZE)); */


  /* printf("Read header: %d CHR and %d PRG\n", header->chr_count, header->prg_count); */

  /* // let's read in the first bank and dump the first tile */
  /* assert(header->chr_count >= 1); */

  /* neschrbank_t *chr_bank = (neschrbank_t*)malloc(sizeof(neschrbank_t)); */

  /* assert(sizeof(chr_bank->tile[0]) == NES_CHR_TILE_SIZE); */

  /* fseek(ifile, nes_chr_offset(header, 0), SEEK_SET); */

  /* // now read */
  /* if (! fread(chr_bank, 1, NES_CHR_BANK_SIZE, ifile)) { */
    /* perror("Failed to read CHR bank.\n"); */
    /* exit(EXIT_FAILURE); */
  /* } */

  /* char tile[NES_CHR_TILE_SIZE]; */
  /* strncpy(tile, chr_bank->tile[0], NES_CHR_TILE_SIZE); */

  /* printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", tile[0], tile[1], tile[2], tile[3], tile[4], tile[5], tile[6], tile[7]); */

  return 0;
}
