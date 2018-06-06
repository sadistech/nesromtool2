#include <assert.h>
#include "nrt.h"

int main() {
  nrt_prgbank_list_t *list = nrt_prgbank_list_create(NULL, NULL);

  assert(list);
}
