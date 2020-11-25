#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "mman.h"

void* kmalloc(uint nbytes);
void kmfree(uint addr);
void nodedump(struct mmap_region* mp);

void *
mmap(void *addr, int length, int prot, int flags, int fd, int offset)
{
  cprintf("prot = %d", prot);
  int mmap_start = KERNBASE/2;
  struct proc* p = myproc();
  //pde_t *pgdir = p->pgdir;
  int mapped_addr;
  struct mmap_region *mmap_region = kmalloc(sizeof(struct mmap_region));
  //cprintf("storing node at %d\n", (uint)mmap_region);
  if (mmap_region == 0){
    //cprintf("error in kmalloc \n");
  }

  if ((uint)p->mmap_region == 0){
    mapped_addr = PGROUNDUP(mmap_start);
    p->mmap_region = mmap_region;
    mmap_region->addr = mapped_addr;
    mmap_region->length = length;
    mmap_region->prot = prot;
    mmap_region->type = flags;
    mmap_region->fd = fd;
    mmap_region->offset = offset;
    mmap_region->next = 0;

  } else {
    struct mmap_region* mp = p->mmap_region;
    while(mp->next != 0){
      mp = mp->next;
    }
    ////cprintf("here \n");
    mapped_addr = PGROUNDUP(mp->addr + mp->length);
    mp->next = mmap_region;
    mmap_region->addr = mapped_addr;
    mmap_region->length = length;
    mmap_region->prot = prot;
    mmap_region->type = flags;
    mmap_region->fd = fd;
    mmap_region->offset = offset;
    mmap_region->next = 0;
  }
  nodedump(mmap_region);
  if (mmap_region->type == MAP_FILE) {
    cprintf("here\n");
    struct file *f = p->ofile[fd];
    fileread(f, (char *)mapped_addr, length);
    cprintf("in mmap_start: %s", (char *)mapped_addr);
  }

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

int
msync(void *addr, uint length)
{
  //cprintf("unmap %d\n", (uint)addr);
  struct proc* p = myproc();
  struct mmap_region* mp = p->mmap_region;
  int start = (uint)addr;
  //nodedump(mmap_region);
  if (mp->addr==0){
    return 0;
  }

  while(mp->addr != start){
    mp = mp->next;
  }
  struct file *f = p->ofile[mp->fd];
  fileseek(f, mp->offset);
  filewrite(f, (char *)start, length);


  return 0;
}



void nodedump(struct mmap_region* mp) {
  cprintf("node located at %d \n", (uint)mp);
  cprintf("next %d \n", (uint)mp->next);
  cprintf("addr %d \n", mp->addr);
  cprintf("length %d \n", mp->length);
  cprintf("prot = %d\n", mp->prot);
  cprintf("flags = %d\n", mp->type);
  cprintf("fd = %d\n", mp->fd);
  cprintf("offset = %d\n", mp->offset);

}
