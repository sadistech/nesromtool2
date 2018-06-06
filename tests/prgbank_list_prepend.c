#include <assert.h>
#include "nrt.h"

int main() {
  nrt_prgbank_list *head = nrt_prgbank_list_create(NULL, NULL);

  nrt_prgbank *a = NRT_PRG_ALLOC;

  nrt_prgbank_list *new_head = nrt_prgbank_list_prepend(head, a);

  assert(new_head->prg == a);
}
