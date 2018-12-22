#include "common.h"
#include "syscall.h"
#include "fs.h"
#include "proc.h"

size_t ramdisk_write(const void *buf, size_t offset, size_t len);
void naive_uload(PCB *pcb, const char *filename);
int mm_brk(uintptr_t new_brk); //add

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  switch (a[0]) {
    case(SYS_yield):_yield();break;
    case(SYS_exit):
    	_halt(0);
    	//naive_uload(NULL,"/bin/init");
    	break;
    case(SYS_write):
    	//Log("SYS_write");
    	/*if(a[1] == 1 || a[1] == 2){
    		char* addr = (char*)a[2];
    		while(a[3]--) _putc(*addr++);
    		c->GPRx = a[3];
    	}*/
    	c->GPRx = fs_write(a[1],(void*)a[2],a[3]);
    	break;
    	//panic("Unhandled syscall ID = %d %d %d %d", a[0],a[1],a[2],a[3]);
    case(SYS_brk):
    	//Log("New program break = 0x%x\n",(uintptr_t)a[1]);
    	mm_brk((uintptr_t)a[1]);
    	c->GPRx = 0;
    	break;
    case(SYS_open):
    	c->GPRx = fs_open((const char*)a[1],a[2],a[3]);
    	//Log("SYS_open returns %d\n",c->GPRx);
    	break;
    case(SYS_read):
    	c->GPRx = fs_read(a[1],(void*)a[2],a[3]);
    	//Log("SYS_read returns %d\n",c->GPRx);
    	break;
    case(SYS_close):
    	c->GPRx = fs_close(a[1]);
    	//Log("SYS_close returns %d\n",c->GPRx);
    	break;
    case(SYS_lseek):
    	c->GPRx = fs_lseek(a[1],a[2],a[3]);
    	//Log("SYS_lseek returns %d\n",c->GPRx);
    	break;
    case(SYS_execve):
    	naive_uload(NULL,(const char*)a[1]);
    	break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
