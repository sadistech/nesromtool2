#include <assert.h>
#include "nrt.h"

int main()
{
  nrt_chr_list *list = nrt_chr_list_create();

  assert(list->count == 0);
  assert(list->list == NULL);

  nrt_chrbank* a = NRT_CHR_ALLOC;
  nrt_chrbank* b = NRT_CHR_ALLOC;
  nrt_chrbank* c = NRT_CHR_ALLOC;

  nrt_chr_list_append(list, a);
  nrt_chr_list_append(list, b);
  nrt_chr_list_append(list, c);

  nrt_chr_listitem* item = nrt_chr_list_delete(list, 1);

  assert( item->chr == b );
  assert( list->list->chr == a );
  assert( list->list->next->chr == c );
  assert( nrt_chr_listitem_count(list->list) == 2 );
  assert( list->count == 2 );

  nrt_chr_list_free(list);
}

