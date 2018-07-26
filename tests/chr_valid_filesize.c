#include <stdio.h>
#include <assert.h>
#include "nrt.h"
#include "test.h"

int main() {
  test_init();

  struct stat *filestat = (struct stat*)calloc(1, sizeof(struct stat));
  char *bank_path = path_join(fixture_dir, "bank0.chr");

  assert(stat(bank_path, filestat) == 0);

  // this file should be correct size
  assert(nrt_chr_valid_filesize(filestat));

  char *nobank_path = path_join(fixture_dir, "title.nes");

  assert(stat(nobank_path, filestat) == 0);

  // this file is not the correct size
  assert(!nrt_chr_valid_filesize(filestat));

}
