#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"
#include "mmu.h"
#include "mman.h"

/* Testing Anonymous mmap with PROT_WRITE*/

int
main(int argc, char *argv[])
{
  int size =  10;  /* we need 10 bytes */
 
  // mmap
  char *addr = (char*)0x4000;
  char* str = mmap(addr, size,  PROT_WRITE/*prot*/, MAP_ANONYMOUS, -1/*fd*/, 0/*offset*/);

  if (str<=0)
  {
    printf(1, "XV6_TEST_OUTPUT : mmap failed\n");
    exit();
  }
  printf(1, "XV6_TEST_OUTPUT : mmap good\n");


  printf(1, "XV6_TEST_OUTPUT : Strlen Before modification: %d\n", strlen((char*)str));

  strcpy(str, "012345");

  printf(1, "XV6_TEST_OUTPUT : str = %s\n", (char*)str);
  printf(1, "XV6_TEST_OUTPUT : Strlen After modification: %d\n", strlen((char*)str));


  // munmap
  int rv = munmap(str, size);
  if (rv < 0) {
    printf(1, "XV6_TEST_OUTPUT : munmap failed\n");
    exit();
  }

  printf(1, "XV6_TEST_OUTPUT : munmap good\n");
  exit();
}
