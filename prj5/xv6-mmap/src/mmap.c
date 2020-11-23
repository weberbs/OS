#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

void* kmalloc(uint nbytes);
void kmfree(uint addr);
void nodedump(struct mmap_region* mp);

void *
mmap(void *addr, int length, int prot, int flags, int fd, int offset)
{
  int mmap_start = KERNBASE/2;
  struct proc* p = myproc();
  pde_t *pgdir = p->pgdir;
  int mapped_addr;
  struct mmap_region *mmap_region = kmalloc(sizeof(struct mmap_region));
  //cprintf("storing node at %d\n", (uint)mmap_region);
  if (mmap_region == 0){
    //cprintf("error in kmalloc \n");
  }

  if ((uint)p->mmap_region == 0){
    mapped_addr = PGROUNDUP(mmap_start);
    allocuvm(pgdir, mapped_addr, mapped_addr + length);
    p->mmap_region = mmap_region;
    mmap_region->addr = mapped_addr;
    mmap_region->length = length;
    mmap_region->type = 0;
    mmap_region->offset = 0;
    mmap_region->next = 0;

  } else {
    struct mmap_region* mp = p->mmap_region;
    while(mp->next != 0){
      mp = mp->next;
    }
    ////cprintf("here \n");
    mapped_addr = PGROUNDUP(mp->addr + mp->length);
    allocuvm(pgdir, mapped_addr, mapped_addr + length);
    mp->next = mmap_region;
    mmap_region->addr = mapped_addr;
    mmap_region->length = length;
    mmap_region->type = 0;
    mmap_region->offset = 0;
    mmap_region->next = 0;
  }
  //nodedump(mmap_region);
  return (void *)mapped_addr;
}

int
munmap(void *addr, uint length)
{
  //cprintf("unmap %d\n", (uint)addr);
  struct proc* p = myproc();
  pde_t *pgdir = p->pgdir;
  struct mmap_region* mmap_region = myproc()->mmap_region;
  struct mmap_region* prev = mmap_region;
  int start = (uint)addr;
  //nodedump(mmap_region);
  if (mmap_region->addr==0){
    return 0;
  }

  if(mmap_region->addr == start) {
    //cprintf("calling dealloc from %d to %d first node\n",length + start, start );
    deallocuvm(pgdir, start + length, start);
    if (mmap_region->next == 0){
      kmfree((uint)mmap_region);
      p->mmap_region = 0;
    } else {
      p->mmap_region = mmap_region->next;
      kmfree((uint)mmap_region);
    }
    return 0;
  } else {
    //cprintf("Here\n");
    while(mmap_region->addr != start){
      prev = mmap_region;
      mmap_region = mmap_region->next;
    }
    //cprintf("calling dealloc from %d to %d\n",length + start, start );
    deallocuvm(pgdir, start + length, start);
    prev->next = mmap_region->next;
    kmfree((uint)mmap_region);
    return 0;
  }
}

void nodedump(struct mmap_region* mp) {
  cprintf("node located at %d \n", (uint)mp);
  cprintf("addr %d \n", mp->addr);
  cprintf("length %d \n", mp->length);
  cprintf("next %d \n", (uint)mp->next);

}
