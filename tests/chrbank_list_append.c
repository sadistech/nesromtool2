#include <assert.h>
#include "nrt.h"

int main()
{
  nrt_chr_list *list = nrt_chr_list_create();

  assert(list->count == 0);
  assert(list->list == NULL);

  nrt_chrbank* a = NRT_CHR_ALLOC;
  nrt_chrbank* b = NRT_CHR_ALLOC;

  // first we prepend an item.
  nrt_chr_list_append(list, a);

  assert(list->list->chr == a);

  // then another
  nrt_chr_list_append(list, b);

  assert(list->list->chr == a);
  assert(list->list->next->chr == b);
}

