#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"

int global;

void threadfunc(void *arg1, void *arg2) {
  *(int *)arg2 = (int)arg1;
  exit();
}
void threadfunc2(void *arg1, void *arg2) {
  global = 12345;
  exit();
}

int main(int argc, char *argv[])
{
  int i = 0;
  int thread1 = thread_create(threadfunc, (void*)31415, (void *)&i);
  int thread2 = thread_create(threadfunc2, 0, 0);
  int r1 = thread_join();
  int r2 = thread_join();
  printf(1, "thread 1 = %d thread 2 = %d\n", thread1, thread2);
  printf(1, "XV6_TEST_OUTPUT : global = %d i = %d\n", global, i);
  printf(1, "reaped thread 1 = %d thread 2 = %d\n", r1, r2);
  exit();
}
