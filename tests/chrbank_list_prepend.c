#include <assert.h>
#include "nrt.h"

int main() {
  nrt_chrbank_list *head = nrt_chrbank_list_create(NULL, NULL);

  nrt_chrbank *a = NRT_CHR_ALLOC;

  nrt_chrbank_list *new_head = nrt_chrbank_list_prepend(head, a);

  assert(new_head->chr == a);
}
