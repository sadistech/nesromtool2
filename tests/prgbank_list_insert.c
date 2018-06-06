#include <assert.h>
#include "nrt.h"

int main() {
  nrt_prgbank_list_t *c = nrt_prgbank_list_create(NULL, NULL);
  nrt_prgbank_list_t *a = nrt_prgbank_list_create(NULL, c);
  nrt_prgbank_list_t *head = nrt_prgbank_list_create(NULL, a);

  nrt_prgbank_t *b = NRT_PRG_ALLOC;

  assert( nrt_prgbank_list_count(head) == 3 );

  head = nrt_prgbank_list_insert(head, b, 2);

  assert( nrt_prgbank_list_count(head) == 4 );

  assert( head->next == a );
  assert( a->next->prg == b );
  assert( a->next->next == c );
}
