#include <assert.h>
#include "nrt.h"

int main() {
  nrt_prgbank_list *c = nrt_prgbank_list_create(NULL, NULL);
  nrt_prgbank_list *a = nrt_prgbank_list_create(NULL, c);
  nrt_prgbank_list *head = nrt_prgbank_list_create(NULL, a);

  nrt_prgbank *b = NRT_PRG_ALLOC;

  assert( nrt_prgbank_list_count(head) == 3 );

  head = nrt_prgbank_list_insert(head, b, 2);

  assert( nrt_prgbank_list_count(head) == 4 );

  assert( head->next == a );
  assert( a->next->prg == b );
  assert( a->next->next == c );
}
