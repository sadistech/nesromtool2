#include <assert.h>
#include "nrt.h"

int main() {
  nrt_prgbank_list *list = nrt_prgbank_list_create(NULL, NULL);

  assert(list);
}
