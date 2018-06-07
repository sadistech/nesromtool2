#include <assert.h>
#include "nrt.h"

int main() {
  nrt_chrbank_list *list = nrt_chrbank_list_create(NULL, NULL);

  assert(list);
}
