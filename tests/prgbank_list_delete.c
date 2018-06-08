#include <assert.h>
#include "nrt.h"

int main()
{
  nrt_prg_list *list = nrt_prg_list_create();

  assert(list->count == 0);
  assert(list->list == NULL);

  nrt_prgbank* a = NRT_PRG_ALLOC;
  nrt_prgbank* b = NRT_PRG_ALLOC;
  nrt_prgbank* c = NRT_PRG_ALLOC;

  nrt_prg_list_append(list, a);
  nrt_prg_list_append(list, b);
  nrt_prg_list_append(list, c);

  nrt_prg_listitem* item = nrt_prg_list_delete(list, 1);

  assert( item->prg == b );
  assert( list->list->prg == a );
  assert( list->list->next->prg == c );
  assert( nrt_prg_listitem_count(list->list) == 2 );
  assert( list->count == 2 );

  nrt_prg_list_free(list);
}

