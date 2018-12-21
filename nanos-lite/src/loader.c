#include "proc.h"
#include "fs.h"
#define DEFAULT_ENTRY 0x8048000
#define PAGE_SIZE 4096


#define PGSHFT    12      // log2(PGSIZE)
#define PTXSHFT   12      // Offset of PTX in a linear address
#define PDXSHFT   22      // Offset of PDX in a linear address
#define PDX(va)     (((uint32_t)(va) >> PDXSHFT) & 0x3ff)
#define PTX(va)     (((uint32_t)(va) >> PTXSHFT) & 0x3ff)
#define OFF(va)     ((uint32_t)(va) & 0xfff)


size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t get_ramdisk_size();
int _protect(_Protect *p); //add
void* new_page(size_t nr_page);//add
int _map(_Protect *p, void *va, void *pa, int mode); //add 

static uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename,0,0);
  //size_t size = fs_filesz(fd);
  int num_page = 1;
 // printf("num_page = %d\n",num_page);
  int i;
  for(i = 0;i < num_page;i++){
      void* ppage = new_page(1);
    //  printf("in _map, va = %x, pa = %x ",(void*)DEFAULT_ENTRY+i*PAGE_SIZE,ppage);
      _map(&(pcb->as),(void*)DEFAULT_ENTRY+i*PAGE_SIZE,ppage,1);
      //if(ret) ppage += PAGE_SIZE; //a physical page was allocated for page table
      //printf("write into: %x\n",ppage);
      //printf("_map rets = 0x%x\n",(void*)ret);
      fs_read(fd,ppage,PAGE_SIZE);
  }
  
  //void* buf = (uintptr_t*)DEFAULT_ENTRY;
  
  //printf("fd = %d,fs_filesz(fd) = %d\n",fd,fs_filesz(fd));
  //fs_read(fd,buf,fs_filesz(fd));
  fs_close(fd);
  //ramdisk_read(buf,0,get_ramdisk_size());
  return DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
  //printf("loader.c: in context_kload(),pcb->cp->irq = %x\n",pcb->cp->irq);
}

void context_uload(PCB *pcb, const char *filename) {
  _protect(&(pcb->as));
  //printf("ret = %x\n",ret);
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
