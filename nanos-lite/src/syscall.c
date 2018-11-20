#include "common.h"
#include "syscall.h"


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
    	if(a[1] == 1 || a[1] == 2){
    		while(a[3]--) {
    			char ch = *((uintptr_t*)a[1]);
    			_putc(ch);
    		}
    	}
    	break;
    	//panic("Unhandled syscall ID = %d %d %d %d", a[0],a[1],a[2],a[3]);
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
