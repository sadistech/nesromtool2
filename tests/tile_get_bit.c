#include "nrt.h"
#include <assert.h>

int main() {
  //                  01234567
  unsigned char a = 0b00001111;

  assert( NRT_GET_BIT(0, a) == 0 );
  assert( NRT_GET_BIT(1, a) == 0 );
  assert( NRT_GET_BIT(2, a) == 0 );
  assert( NRT_GET_BIT(3, a) == 0 );
  assert( NRT_GET_BIT(4, a) == 1 );
  assert( NRT_GET_BIT(5, a) == 1 );
  assert( NRT_GET_BIT(6, a) == 1 );
  assert( NRT_GET_BIT(7, a) == 1 );
}

