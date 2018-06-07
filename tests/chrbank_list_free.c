#include <assert.h>
#include "nrt.h"

int main() {
  nrt_chrbank_list *head = nrt_chrbank_list_create(NULL, NULL);

  assert(head);

  nrt_chrbank_list_free(head);

  // let's just hope that nothing blows up.
}

