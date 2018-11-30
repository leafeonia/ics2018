#include "proc.h"
#include "fs.h"
#define DEFAULT_ENTRY 0x4000000

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t get_ramdisk_size();

static uintptr_t loader(PCB *pcb, const char *filename) {
  void* buf = (uintptr_t*)DEFAULT_ENTRY;
  int fd = fs_open(filename);
  //printf("fd = %d,fs_filesz(fd) = %d\n",fd,fs_filesz(fd));
  fs_read(fd,buf,fs_filesz(fd));
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

  pcb->tf = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->tf = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
