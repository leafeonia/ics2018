#include "common.h"
#include "syscall.h"
#include "fs.h"

//extern int exec_open(const char*);

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  switch (a[0]) {
    case(SYS_yield):_yield();break;
    case(SYS_exit):_halt(0);break;
    case(SYS_write):
    	//Log("SYS_write");
    	if(a[1] == 1 || a[1] == 2){
    		char* addr = (char*)a[2];
    		while(a[3]--) _putc(*addr++);
    	}
    	break;
    	//panic("Unhandled syscall ID = %d %d %d %d", a[0],a[1],a[2],a[3]);
    case(SYS_brk):
    	c->GPRx = 0;
    	break;
    case(SYS_open):
    	Log("SYS_open");
    	//exec_open((const char*)a[1]);
    	break;
    	
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
