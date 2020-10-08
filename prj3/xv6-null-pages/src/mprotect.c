#include "types.h"
#include "stat.h"
#include "user.h"
#include "stddef.h"

int main(void){
  int a, b;
  int *pi;

  a = 5;
  pi = &a;
  b = *pi;
  pi = NULL;
  printf(1,"a is %i\n", a);
  printf(1,"address of a is %p\n", &a);
  printf(1,"b is %i\n", b);
  printf(1,"address of b is %p\n", &b);
  printf(1,"address of pi is %p\n", &pi);
  printf(1,"pi points to the address %p\n", pi);
  printf(1,"integer at %p is %i\n",pi, *pi);

  return 0;
}
