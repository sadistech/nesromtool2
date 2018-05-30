#include "util.h"

/*
 * return whether the file is ok or not
 * pass it a pre-allocated nesrom_status_t
 */
void nrt_validate_file(FILE *rom, nrt_error_t *error) {
  nrt_header_t *header = NRT_HEADER_ALLOC;
  char msg[256] = "";

  if (! fread(header, 1, sizeof(nrt_header_t), rom) ) {
    nrt_append_error(error, "Failed to read header from file.");
    return;
  }

  if (! nrt_validate_header_magic_word(header)) {
    sprintf(msg, "Magic word does not match: %s", header->magic_word);
    nrt_append_error(error, msg);
  }

  if (! nrt_validate_header_prg_count(header)) {
    sprintf(msg, "PRG count is invalid: %d", header->prg_count);
    nrt_append_error(error, msg);
  }
}
