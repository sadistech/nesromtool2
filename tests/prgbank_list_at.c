#include <assert.h>
#include "nrt.h"

int main() {
  nrt_prgbank_list_t *c = nrt_prgbank_list_create(NULL, NULL);
  nrt_prgbank_list_t *b = nrt_prgbank_list_create(NULL, c);
  nrt_prgbank_list_t *a = nrt_prgbank_list_create(NULL, b);
  nrt_prgbank_list_t *head = nrt_prgbank_list_create(NULL, a);

  assert( nrt_prgbank_list_at(head, 0) == head );
  assert( nrt_prgbank_list_at(head, 1) == a );
  assert( nrt_prgbank_list_at(head, 2) == b );
  assert( nrt_prgbank_list_at(head, 3) == c );
  assert( nrt_prgbank_list_at(head, 4) == NULL );
}

