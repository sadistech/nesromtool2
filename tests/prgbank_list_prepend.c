#include <assert.h>
#include "nrt.h"

int main() {
  nrt_prgbank_list_t *head = nrt_prgbank_list_create(NULL, NULL);

  nrt_prgbank_t *a = NRT_PRG_ALLOC;

  nrt_prgbank_list_t *new_head = nrt_prgbank_list_prepend(head, a);

  assert(new_head->prg == a);
}
