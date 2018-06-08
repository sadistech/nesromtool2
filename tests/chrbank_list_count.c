#include <assert.h>
#include "nrt.h"

int main()
{
  nrt_chr_list* list = nrt_chr_list_create();

  assert(list->count == 0);
  assert(list->list == NULL);

  nrt_chrbank* a = NRT_CHR_ALLOC;
  nrt_chrbank* b = NRT_CHR_ALLOC;
  nrt_chrbank* c = NRT_CHR_ALLOC;

  nrt_chr_list_append(list, a);
  nrt_chr_list_append(list, b);
  nrt_chr_list_append(list, c);

  assert( nrt_chr_listitem_count(list->list) == 3 );
  assert( list->count == 3 );

  nrt_chr_list_free(list);
}
