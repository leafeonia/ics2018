#include <unistd.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <sys/time.h>
#include <assert.h>
#include <time.h>
#include "syscall.h"
#include <stdio.h>

#if defined(__ISA_X86__)
intptr_t _syscall_(int type, intptr_t a0, intptr_t a1, intptr_t a2){
  int ret = -1;
  asm volatile("int $0x80": "=a"(ret): "a"(type), "b"(a0), "c"(a1), "d"(a2));
  return ret;
}
#elif defined(__ISA_AM_NATIVE__)
intptr_t _syscall_(int type, intptr_t a0, intptr_t a1, intptr_t a2){
  intptr_t ret = 0;
  asm volatile("call *0x100000": "=a"(ret): "a"(type), "S"(a0), "d"(a1), "c"(a2));
  return ret;
}
#else
#error _syscall_ is not implemented
#endif

void _exit(int status) {
  _syscall_(SYS_exit, status, 0, 0);
  while (1);
}

int _open(const char *path, int flags, mode_t mode) {
  return _syscall_(SYS_open, (intptr_t)path, (intptr_t)flags, (intptr_t)mode);
  //_exit(SYS_open);
  //return 0;
}

int _write(int fd, void *buf, size_t count){
  /*
  _syscall_(SYS_write, fd, (intptr_t)buf, count);
  return count;
  */
  return _syscall_(SYS_write, fd, (intptr_t)buf, count);
  //_exit(SYS_write);
  //return 0;
}

extern char end;
intptr_t program_break = (intptr_t)&(end);
//intptr_t program_break;
void *_sbrk(intptr_t increment){
  /*
  if(increment == -1){
    extern intptr_t end;
    program_break = end;
  }
  */
  //printf("_sbrk: program_break:%#x\n", program_break);
  intptr_t original = program_break;
  _syscall_(SYS_brk, program_break+increment, 0, 0);
  program_break += increment;
  return (void*)original;
  /*
  extern uintptr_t end;
  intptr_t original = end;
  _syscall_(SYS_brk, end+increment, 0, 0);
  return original;
  if(program_break == -1){
    program_break = end;
  }
  
  intptr_t now = program_break + increment;
  int term = _syscall_(SYS_brk, now, 0, 0);
  
  if(term != 0)
    assert(0);
  
  program_break = now;
  return (void*)(now-increment);
  if((int)_syscall_(SYS_brk, now, 0, 0) == 0){
    program_break = now;
    return (void*)(now-increment);
  }
  else{
    //_putc('_'); _putc('s'); _putc('b'); _putc('r'); _putc('k'); _putc('\n');
    assert(0);
  }
  */
  //return (void *)-1;
}

int _read(int fd, void *buf, size_t count) {
  //printf("_read: count:%d\n", count);
  return _syscall_(SYS_read, (intptr_t)fd, (intptr_t)buf, (intptr_t)count);
  //_exit(SYS_read);
  //return 0;
}

int _close(int fd) {
  return _syscall_(SYS_close, (intptr_t)fd, 0, 0);
  //_exit(SYS_close);
  //return 0;
}

off_t _lseek(int fd, off_t offset, int whence) {
  return _syscall_(SYS_lseek, (intptr_t)fd, (intptr_t)offset, (intptr_t)whence);
  //_exit(SYS_lseek);
  //return 0;
}

int _execve(const char *fname, char * const argv[], char *const envp[]) {
  _exit(SYS_execve);
  return 0;
}

// The code below is not used by Nanos-lite.
// But to pass linking, they are defined as dummy functions

int _fstat(int fd, struct stat *buf) {
  return 0;
}

int _kill(int pid, int sig) {
  _exit(-SYS_kill);
  return -1;
}

pid_t _getpid() {
  _exit(-SYS_getpid);
  return 1;
}
