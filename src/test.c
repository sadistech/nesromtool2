#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "nrt.h"
#include "nrt/test.h"

test(header_alloc) {
  nrt_header_t *header = NRT_HEADER_ALLOC;

  bool result = !!(header);

  free(header);

  return result;
}

test(header_validate) {
  nrt_header_t *header = NRT_HEADER_ALLOC;
  strncpy(header->magic_word, NRT_MAGIC_WORD, NRT_MAGIC_WORD_SIZE);
  header->prg_count = 1;
  header->chr_count = 1;

  bool result = nrt_validate_header(header);

  free(header);

  return result;
}

test(header_validate_no_prg) {
  nrt_header_t *header = NRT_HEADER_ALLOC;
  strncpy(header->magic_word, NRT_MAGIC_WORD, NRT_MAGIC_WORD_SIZE);
  header->prg_count = 0;
  header->chr_count = 1;

  bool result = !nrt_validate_header(header);

  free(header);

  return result;
}

test(header_validate_bad_magic_word) {
  nrt_header_t *header = NRT_HEADER_ALLOC;
  strncpy(header->magic_word, "FOOD", NRT_MAGIC_WORD_SIZE);
  header->prg_count = 1;
  header->chr_count = 1;

  bool result = !nrt_validate_header(header);

  free(header);

  return result;
}

test(chr_alloc) {
  nrt_chrbank_t *chr = NRT_CHR_ALLOC;

  bool result = !!(chr);
  free(chr);

  return result;
}

test(negative_chr) {
  nrt_header_t *header = NRT_HEADER_ALLOC;

  header->chr_count = 5;

  bool result = !nrt_chr_index_valid(header, -1);
  free(header);

  return result;
}

test(chr_index_too_high) {
  nrt_header_t *header = NRT_HEADER_ALLOC;

  header->chr_count = 5;

  bool result = !nrt_chr_index_valid(header, 5);
  free(header);

  return result;
}

test(chr_index_at_max) {
  nrt_header_t *header = NRT_HEADER_ALLOC;

  header->chr_count = 5;

  bool result = nrt_chr_index_valid(header, 4);
  free(header);

  return result;
}

int main() {
  describe("header");
    it("should alloc a new header", header_alloc);
    it("should validate when valid", header_validate);
    it("should fail validation if no PRG", header_validate_no_prg);
    it("should fail validation if incorrect magic word", header_validate_bad_magic_word);

  describe("chr");
    it("should be able to alloc a chr bank", chr_alloc);
    it("should fail to validate a negative chr index", negative_chr);
    it("should fail to validate a chr index outside of the limit", chr_index_too_high);
    it("should allow accessing the last chr index", chr_index_at_max);

  print_summary();
}
