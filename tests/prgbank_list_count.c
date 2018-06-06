#include <assert.h>
#include "nrt.h"

int main() {
  nrt_prgbank_list_t *head = nrt_prgbank_list_create(NULL, NULL);

  nrt_prgbank_list_t *a = nrt_prgbank_list_create(NULL, NULL);
  nrt_prgbank_list_t *b = nrt_prgbank_list_create(NULL, NULL);
  nrt_prgbank_list_t *c = nrt_prgbank_list_create(NULL, NULL);

  head->next = a;
  a->next = b;
  b->next = c;
  c->next = NULL;

  assert( nrt_prgbank_list_count(head) == 4 );
}
