#include <x86.h>

#define PG_ALIGN __attribute((aligned(PGSIZE)))

static PDE kpdirs[NR_PDE] PG_ALIGN;
static PTE kptabs[PMEM_SIZE / PGSIZE] PG_ALIGN;
static void* (*pgalloc_usr)(size_t);
static void (*pgfree_usr)(void*);

extern uint32_t paddr_read(uint32_t addr, int len);//add

_Area segments[] = {      // Kernel memory mappings
  {.start = (void*)0,          .end = (void*)PMEM_SIZE}
};

#define NR_KSEG_MAP (sizeof(segments) / sizeof(segments[0]))

int _vme_init(void* (*pgalloc_f)(size_t), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  int i;

  // make all PDEs invalid
  for (i = 0; i < NR_PDE; i ++) {
    kpdirs[i] = 0;
  }
  int ret = 0;
  PTE *ptab = kptabs;
  for (i = 0; i < NR_KSEG_MAP; i ++) {
    uint32_t pdir_idx = (uintptr_t)segments[i].start / (PGSIZE * NR_PTE);
    uint32_t pdir_idx_end = (uintptr_t)segments[i].end / (PGSIZE * NR_PTE);
    for (; pdir_idx < pdir_idx_end; pdir_idx ++) {
      // fill PDE
      kpdirs[pdir_idx] = (uintptr_t)ptab | PTE_P;
		
      // fill PTE
      PTE pte = PGADDR(pdir_idx, 0, 0) | PTE_P;
      
      PTE pte_end = PGADDR(pdir_idx + 1, 0, 0) | PTE_P;
      //if(pdir_idx == 31) ret = pte;
      for (; pte < pte_end; pte += PGSIZE) {
        *ptab = pte;
        ptab ++;
      }
      //ret = pte;
    }
    //ret = (int)pdir_idx_end;
  }
  
  set_cr3(kpdirs);
  set_cr0(get_cr0() | CR0_PG);

  return ret;
}

int _protect(_Protect *p) {
  PDE *updir = (PDE*)(pgalloc_usr(1));
  /*int ret = 0;
  ret = (int)updir;*/
  p->pgsize = 4096;
  p->ptr = updir;
  // map kernel space
  for (int i = 0; i < NR_PDE; i ++) {
    updir[i] = kpdirs[i];
  }

  p->area.start = (void*)0x8000000;
  p->area.end = (void*)0xc0000000;
  return 0;
}

void _unprotect(_Protect *p) {
}

static _Protect *cur_as = NULL;

void get_cur_as(_Context *c) {
  c->prot = cur_as;
}

void _switch(_Context *c) {
  set_cr3(c->prot->ptr);
  cur_as = c->prot;
}

int _map(_Protect *p, void *va, void *pa, int mode) {
  PDE* updir = p->ptr;
  uint32_t dir_idx = PDX(va);
  PTE* uptab = NULL;
  //int ret = 0;
  if((updir[dir_idx] & 1) == 0){  //&1 => present bit
  	uptab = (PTE*)(pgalloc_usr(1));
  	updir[dir_idx] = (uintptr_t)uptab | PTE_P;
  	//pa += PGSIZE;
  	//ret = (int)(&updir[dir_idx]);
  }
  else uptab = (PTE*)(updir[dir_idx] & 0xfffff000);//&0xfffff => base
  
  uint32_t tab_idx = PTX(va);
  //ret = (int)(&uptab[0x4a]);
  if((mode & 1) == 1)uptab[tab_idx] = (((uintptr_t)pa & 0xfffff000) | PTE_P);
  else uptab[tab_idx] &= 0xfffffff0;
  //PTE pte = kpdirs[pdir_idx];
  
  //table_index.
  
  return 0;
}

_Context *_ucontext(_Protect *p, _Area ustack, _Area kstack, void *entry, void *args) {
  ustack.end -= 1 * sizeof(uintptr_t);  // 1 = retaddr
  uintptr_t ret = (uintptr_t)ustack.end;
  *(uintptr_t *)ret = 0;

  _Context *c = (_Context*)ustack.end - 1;
  
  c->eip = (uintptr_t)entry;
  c->cs = 8;
  c->prot = p;
  c->eflags = (1 << 9);
  
  return c;
}
