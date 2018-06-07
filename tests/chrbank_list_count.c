#include <assert.h>
#include "nrt.h"

int main() {
  nrt_chrbank_list *head = nrt_chrbank_list_create(NULL, NULL);

  nrt_chrbank_list *a = nrt_chrbank_list_create(NULL, NULL);
  nrt_chrbank_list *b = nrt_chrbank_list_create(NULL, NULL);
  nrt_chrbank_list *c = nrt_chrbank_list_create(NULL, NULL);

  head->next = a;
  a->next = b;
  b->next = c;
  c->next = NULL;

  assert( nrt_chrbank_list_count(head) == 4 );
}
