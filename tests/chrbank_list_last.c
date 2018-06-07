#include <assert.h>
#include "nrt.h"

int main() {
  nrt_chrbank_list *c = nrt_chrbank_list_create(NULL, NULL);
  nrt_chrbank_list *b = nrt_chrbank_list_create(NULL, c);
  nrt_chrbank_list *a = nrt_chrbank_list_create(NULL, b);
  nrt_chrbank_list *head = nrt_chrbank_list_create(NULL, a);

  assert( nrt_chrbank_list_last(head) == c );
}

