#include <stdio.h>
#include <assert.h>
#include "nrt.h"
#include "test.h"

int main() {
  test_init();

  char *bank_path = path_join(fixture_dir, "bank0.chr");

  assert(nrt_chr_valid_filesize(bank_path));

  char *nobank_path = path_join(fixture_dir, "title.nes");

  assert(!nrt_chr_valid_filesize(nobank_path));
}
