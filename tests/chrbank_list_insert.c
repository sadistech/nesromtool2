#include <assert.h>
#include "nrt.h"

int main() {
  nrt_chrbank_list *c = nrt_chrbank_list_create(NULL, NULL);
  nrt_chrbank_list *a = nrt_chrbank_list_create(NULL, c);
  nrt_chrbank_list *head = nrt_chrbank_list_create(NULL, a);

  nrt_chrbank *b = NRT_CHR_ALLOC;

  assert( nrt_chrbank_list_count(head) == 3 );

  head = nrt_chrbank_list_insert(head, b, 2);

  assert( nrt_chrbank_list_count(head) == 4 );

  assert( head->next == a );
  assert( a->next->chr == b );
  assert( a->next->next == c );
}
