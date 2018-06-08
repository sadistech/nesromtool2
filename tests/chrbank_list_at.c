#include <assert.h>
#include "nrt.h"

int main() {
  nrt_chr_list* list = nrt_chr_list_create();

  assert(list->count == 0);
  assert(list->list == NULL);

  nrt_chrbank* a = NRT_CHR_ALLOC;
  nrt_chrbank* b = NRT_CHR_ALLOC;
  nrt_chrbank* c = NRT_CHR_ALLOC;

  nrt_chr_listitem* item;

  nrt_chr_list_append(list, a);
  nrt_chr_list_append(list, b);
  nrt_chr_list_append(list, c);

  item = nrt_chr_listitem_at(list, 0);
  assert( item->chr == a );
  item = nrt_chr_listitem_at(list, 1);
  assert( item->chr == b );
  item = nrt_chr_listitem_at(list, 2);
  assert( item->chr == c );
  item = nrt_chr_listitem_at(list, 3);
  assert( item == NULL );

  nrt_chr_list_free(list);
}

