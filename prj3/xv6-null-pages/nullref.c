#include "types.h"
#include "stat.h"
#include "user.h"

int main(void){
  int a, b; // some integers
  int *pi;     // a pointer to an integer

  a = 5;
  pi = &a; // pi points to a
  b = *pi; // b is now 5
  pi = NULL;
  printf("a is %i\n", a);
  printf("address of a is %p\n", &a);
  printf("b is %i\n", b);
  printf("address of b is %p\n", &b);
  printf("address of pi is %p\n", &pi);
  printf("pi points to the address %p\n", pi);
  printf("integer at %p is %i\n",pi, *pi);

  return 0;
}
