#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used));
static PCB pcb_boot;
PCB *current;

void naive_uload(PCB *pcb, const char *filename);//add
void context_kload(PCB *pcb, void *entry);//add
void context_uload(PCB *pcb, const char *filename);//add

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
  }
}

void init_proc() {
	//naive_uload(NULL,"/bin/init");
	
	//context_kload(&pcb[0],(void*)hello_fun);
	context_uload(&pcb[0], "/bin/event");
	
	//printf("proc.c:in init_proc(): &hello_fun = %x\n",(void*)hello_fun);
	switch_boot_pcb();
}

_Context* schedule(_Context *prev) {
    current->cp = prev;
	//current = &pcb[0];
	current = &pcb[0];//(current == &pcb[0] ? &pcb[1] : &pcb[0]);
	return current->cp;
}
