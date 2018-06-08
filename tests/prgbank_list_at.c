#include <assert.h>
#include "nrt.h"

int main() {
  nrt_prg_list* list = nrt_prg_list_create();

  assert(list->count == 0);
  assert(list->list == NULL);

  nrt_prgbank* a = NRT_PRG_ALLOC;
  nrt_prgbank* b = NRT_PRG_ALLOC;
  nrt_prgbank* c = NRT_PRG_ALLOC;

  nrt_prg_listitem* item;

  nrt_prg_list_append(list, a);
  nrt_prg_list_append(list, b);
  nrt_prg_list_append(list, c);

  item = nrt_prg_listitem_at(list, 0);
  assert( item->prg == a );
  item = nrt_prg_listitem_at(list, 1);
  assert( item->prg == b );
  item = nrt_prg_listitem_at(list, 2);
  assert( item->prg == c );
  item = nrt_prg_listitem_at(list, 3);
  assert( item == NULL );

  nrt_prg_list_free(list);
}

