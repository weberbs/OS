#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

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

static pte_t *
walkpgdir(pde_t *pgdir, const void *va, int alloc)
{
  pde_t *pde;
  pte_t *pgtab;

  pde = &pgdir[PDX(va)];
  if(*pde & PTE_P){
    pgtab = (pte_t*)P2V(PTE_ADDR(*pde));
  } else {
    if(!alloc || (pgtab = (pte_t*)kalloc()) == 0)
      return 0;
    // Make sure all those PTE_P bits are zero.
    memset(pgtab, 0, PGSIZE);
    // The permissions here are overly generous, but they can
    // be further restricted by the permissions in the page table
    // entries, if necessary.
    *pde = V2P(pgtab) | PTE_P | PTE_W | PTE_U;
  }
  return &pgtab[PTX(va)];
}

int
sys_mprotect(void)
{
  int len;
  void *vaddr;
  pte_t *vpg;
  pte_t *pgdir;
  uint ppg;

  //pde_t *pde;
  //pte_t *pgtab;
  if(argint(1, &len) < 0)
    return -1;
  if(argptr(0, (void*)&vaddr, sizeof(vaddr)) < 0)
    return -1;
  if (len <= 0)
    return -1;
  if((uint) vaddr % PGSIZE != 0)
    return -1;

  pgdir = myproc() -> pgdir;
  vpg = walkpgdir(pgdir, vaddr, 0);
  ppg = V2P(vpg);
  *vpg = ppg;
  *vpg &= ~PTE_W;
  return 0;
}

int
sys_munprotect(void)
{
  int len;
  void *addr;
  //pte_t *pa;
  //pte_t *pgdir;
  //pde_t *pde;
  //pte_t *pgtab;
  if(argint(1, &len) < 0)
    return -1;
  if(argptr(0, (void*)&addr, sizeof(addr)) < 0)
    return -1;
  if (len <= 0)
    return -1;
  if((uint) addr % PGSIZE != 0)
    return -1;

  /*pgdir = myproc() -> pgdir;
  for (int i = 0; i < len; i++){
    pde = &pgdir[PDX(addr)];
    pgtab = (pte_t*)P2V(PTE_ADDR(*pde));
    pa = &pgtab[PTX(addr)];
    *pa = | PTE_W;
    addr += PGSIZE;
  }*/

  return 0;
}
