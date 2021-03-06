#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

void* kmalloc(uint nbytes);
void kmfree(uint addr);
void *mmap(uint addr, int length, int prot, int flags, int fd, int offset);
int munmap(uint addr, uint length);
int msync(uint addr, uint length);


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

void *
sys_kmalloc(void)
{
  int n;
  argint(0, &n);
  return kmalloc(n);
}

void
sys_kmfree(void)
{
  int n;
  argint(0, &n);
  kmfree(n);
  return;
}

void *
sys_mmap(void)
{
  int addr, length, prot, flags, fd, offset;
  argint(0, &addr);
  argint(1, &length);
  argint(2, &prot);
  argint(3, &flags);
  argint(4, &fd);
  argint(5, &offset);
  return mmap(addr, length, prot, flags, fd, offset);
}

int
sys_munmap(void)
{
  int addr, length;
  if(argint(0, &addr) < 0)
    return -1;
  if(argint(1, &length) < 0)
    return -1;
  return munmap(addr, length);
}

int
sys_msync(void)
{
  int addr, length;
  if(argint(0, &addr) < 0)
    return -1;
  if(argint(1, &length) < 0)
    return -1;
  return msync(addr, length);

}
