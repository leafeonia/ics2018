#include "memory.h"
#include "proc.h"

static void *pf = NULL;
extern PCB *current;//add
int _map(_Protect *p, void *va, void *pa, int mode); //add 
#define PGSIZE 4096

void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  Log("new pages free : %p", p);
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t new_brk) {
  current->cur_brk = new_brk;
  if(new_brk > current->max_brk){
    int cnt = (new_brk - current->max_brk) / PGSIZE + 1;
    int i;
    for(i = 0;i < cnt;i++){
    	void* ppage = new_page(1);
  		_map(&(current->as),(void*)current->max_brk,ppage,1);
  		printf("in _map, va = %x, pa = %x ",(void*)current->max_brk,ppage);
  		current->max_brk += PGSIZE;
    }
  	
  }
  return 0;
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
  //printf("ret of vme_init = 0x%x\n",ret);
}
