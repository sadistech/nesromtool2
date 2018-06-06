#include <assert.h>
#include "nrt.h"

int main() {
  nrt_prgbank_list *head = nrt_prgbank_list_create(NULL, NULL);

  nrt_prgbank_list *a = nrt_prgbank_list_create(NULL, NULL);
  nrt_prgbank_list *b = nrt_prgbank_list_create(NULL, NULL);
  nrt_prgbank_list *c = nrt_prgbank_list_create(NULL, NULL);

  head->next = a;
  a->next = b;
  b->next = c;
  c->next = NULL;

  assert( nrt_prgbank_list_count(head) == 4 );
}
