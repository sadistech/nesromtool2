#include <assert.h>
#include "nrt.h"

int main()
{
  nrt_prg_list* list = nrt_prg_list_create();

  nrt_prgbank* a = NRT_PRG_ALLOC;
  nrt_prgbank* b = NRT_PRG_ALLOC;
  nrt_prgbank* c = NRT_PRG_ALLOC;

  assert( nrt_prg_listitem_last(list) == NULL );

  nrt_prg_listitem* last;

  nrt_prg_list_append(list, a);
  last = nrt_prg_listitem_last(list);
  assert( last->prg == a );

  nrt_prg_list_append(list, b);
  last = nrt_prg_listitem_last(list);
  assert( last->prg == b );

  nrt_prg_list_append(list, c);
  last = nrt_prg_listitem_last(list);
  assert( last->prg == c );

  nrt_prg_list_free(list);
}

