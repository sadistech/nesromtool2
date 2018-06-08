#include <assert.h>
#include "nrt.h"

int main()
{
  nrt_chr_list* list = nrt_chr_list_create();

  nrt_chrbank* a = NRT_CHR_ALLOC;
  nrt_chrbank* b = NRT_CHR_ALLOC;
  nrt_chrbank* c = NRT_CHR_ALLOC;

  assert( nrt_chr_listitem_last(list) == NULL );

  nrt_chr_listitem* last;

  nrt_chr_list_append(list, a);
  last = nrt_chr_listitem_last(list);
  assert( last->chr == a );

  nrt_chr_list_append(list, b);
  last = nrt_chr_listitem_last(list);
  assert( last->chr == b );

  nrt_chr_list_append(list, c);
  last = nrt_chr_listitem_last(list);
  assert( last->chr == c );

  nrt_chr_list_free(list);
}

