#include <assert.h>
#include "nrt.h"

int main() {
  nrt_prgbank_list *head = nrt_prgbank_list_create(NULL, NULL);

  nrt_prgbank *a = NRT_PRG_ALLOC;

  assert( nrt_prgbank_list_count(head) == 1 );

  nrt_prgbank_list_append(head, a);

  assert( nrt_prgbank_list_count(head) == 2 );

  assert( head->next );
  assert( head->next->prg == a );
  assert( head->next->next == NULL );
}

