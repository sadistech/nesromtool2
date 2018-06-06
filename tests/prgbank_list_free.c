#include <assert.h>
#include "nrt.h"

int main() {
  nrt_prgbank_list *head = nrt_prgbank_list_create(NULL, NULL);

  assert(head);

  nrt_prgbank_list_free(head);

  // let's just hope that nothing blows up.
}

