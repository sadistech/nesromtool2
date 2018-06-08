#include <assert.h>
#include "nrt.h"

int main()
{
  nrt_prg_list* list = nrt_prg_list_create();

  assert(list->count == 0);
  assert(list->list == NULL);

  nrt_prgbank* a = NRT_PRG_ALLOC;
  nrt_prgbank* b = NRT_PRG_ALLOC;
  nrt_prgbank* c = NRT_PRG_ALLOC;

  // first we prepend an item.
  nrt_prg_list_insert(list, a, 0);
  assert( list->list->prg == a );

  // then another
  nrt_prg_list_insert(list, c, 1);
  assert( list->list->next->prg == c );

  nrt_prg_list_insert(list, b, 1);
  assert( list->list->next->prg == b );

  nrt_prg_list_free(list);
}
