#include <assert.h>
#include "nrt.h"

int main()
{
  nrt_prg_list *list = nrt_prg_list_create();

  assert(list->count == 0);
  assert(list->list == NULL);

  nrt_prgbank* a = NRT_PRG_ALLOC;
  nrt_prgbank* b = NRT_PRG_ALLOC;

  // first we prepend an item.
  nrt_prg_list_append(list, a);

  assert(list->list->prg == a);

  // then another
  nrt_prg_list_append(list, b);

  assert(list->list->prg == a);
  assert(list->list->next->prg == b);
}

