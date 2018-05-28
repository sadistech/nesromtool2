#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define NES_HEADER_SIZE 16
#define NES_MAGIC_WORD "NES\x1a"
#define NES_MAGIC_WORD_SIZE 4

#define NES_PRG_BANK_SIZE 16384
#define NES_PRG_BANK_MAX_COUNT 64

#define NES_CHR_BANK_SIZE 8192
#define NES_CHR_TILE_SIZE 16
#define NES_CHR_TILE_COUNT 512 /* NES_CHR_BANK_SIZE / NES_CHR_TILE_SIZE */

#define nes_chr_offset(header, chr_index) NES_HEADER_SIZE + (header->prg_count * NES_PRG_BANK_SIZE) + (chr_index * NES_CHR_BANK_SIZE)

typedef struct nesheader_t {
  char magic_word[NES_MAGIC_WORD_SIZE];
  unsigned char prg_count;
  unsigned char chr_count;
  unsigned char the_rest[10];
} nesheader_t;

typedef struct neschrbank_t {
  unsigned char tile[NES_CHR_TILE_COUNT][NES_CHR_TILE_SIZE];
} neschrbank_t;

typedef struct nesprgbank_t {
  unsigned char data[NES_PRG_BANK_SIZE];
} nesprgbank_t;

/* typedef struct nesverify_result_t { */
  /* unsigned char has_error = 0; */
  /* char *msg; */
/* } nesverify_result_t; */

// validate, chr, prg, tile

bool nesvalidate_header_magic_word(nesheader_t* header) {
  return strncmp(header->magic_word, NES_MAGIC_WORD, NES_MAGIC_WORD_SIZE) == 0;
}

bool nesvalidate_header_prg_count(nesheader_t* header) {
  return header->prg_count > 0 && header->prg_count <= NES_PRG_BANK_MAX_COUNT;
}

/* nesverify_result_t* nesverify (filepath) { */
  /* nesverify_result_t = (nesverify_result_t*)malloc(sizeof(nesverify_result_t)); */

  /* nesheader_t *header = (nesheader_t*)malloc(sizeof(nesheader_t)); */

/* } */

// verify <path>
void action_verify(char *filepath) {
  FILE *ifile = NULL;
  nesheader_t *header = (nesheader_t*)malloc(sizeof(nesheader_t));

  if (!(ifile = fopen(filepath, "r+"))) {
    fprintf(stderr, "Failed to open file.");
    exit(EXIT_FAILURE);
  }

  if (! fread(header, 1, sizeof(nesheader_t), ifile )) {
    fprintf(stderr, "Failed to read header.\n");
    exit(EXIT_FAILURE);
  }

  bool (*checks[2])(nesheader_t *) = {
    &nesvalidate_header_magic_word,
    &nesvalidate_header_prg_count
  };

  int i = 0;
  bool result = true;
  for (i = 0; i < sizeof(checks); i++) {
    if (! checks[i](header)) {
      result = false;
    }
  }

  if (result) {
    printf("OK\n");
    exit(EXIT_SUCCESS);
  } else {
    printf("FAIL\n");
    exit(EXIT_FAILURE);
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
