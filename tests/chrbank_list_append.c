#include <assert.h>
#include "nrt.h"

int main() {
  nrt_chrbank_list *head = nrt_chrbank_list_create(NULL, NULL);

  nrt_chrbank *a = NRT_CHR_ALLOC;

  assert( nrt_chrbank_list_count(head) == 1 );

  nrt_chrbank_list_append(head, a);

  assert( nrt_chrbank_list_count(head) == 2 );

  assert( head->next );
  assert( head->next->chr == a );
  assert( head->next->next == NULL );
}

