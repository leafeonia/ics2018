#include <am.h>
#include <x86.h>
#include <stdio.h>
static _Context* (*user_handler)(_Event, _Context*) = NULL;

void vectrap();
void vecnull();
void vecsys();

void get_cur_as(_Context *c);//add
void _switch(_Context *c);//add

_Context* irq_handle(_Context *tf) {
  get_cur_as(tf);
  _Context *next = tf;
  /*printf("tf.eflags = %x\n",tf->eflags);
  printf("tf.cs = %x\n",tf->cs);
  printf("tf.eip = %x\n",tf->eip);
  printf("tf.err = %x\n",tf->err);
  printf("tf.irq = %x\n",tf->irq);
  printf("tf.eax = %x\n",tf->eax); 
  printf("tf.ecx = %x\n",tf->ecx);
  printf("tf.edx = %x\n",tf->edx);
  printf("tf.ebx = %x\n",tf->ebx);
  printf("tf.esp = %x\n",tf->esp);
  printf("tf.ebp = %x\n",tf->ebp);
  printf("tf.esi = %x\n",tf->esi);
  printf("tf.edi = %x\n",tf->edi);*/
  if (user_handler) {
    _Event ev = {0};
    //printf("cte.c. tf->irq = %x\n",tf->irq);
    switch (tf->irq) {
      case 0x81:
      	ev.event = _EVENT_YIELD;break;
      case 0x80:
      	ev.event = _EVENT_SYSCALL;break;
      default: ev.event = _EVENT_ERROR; break;
    }
	//printf("cte.c: tf->esp = %x\n",tf->esp);
    next = user_handler(ev, tf);
    if (next == NULL) {
      next = tf;
    }
  }
  //printf("cte.c: return %x\n",(void*)next);
  _switch(next);
  return next;
}

static GateDesc idt[NR_IRQ];

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  // initialize IDT
  for (unsigned int i = 0; i < NR_IRQ; i ++) {
    idt[i] = GATE(STS_TG32, KSEL(SEG_KCODE), vecnull, DPL_KERN);
  }

  // -------------------- system call --------------------------
  idt[0x80] = GATE(STS_TG32, KSEL(SEG_KCODE), vecsys, DPL_KERN);
  idt[0x81] = GATE(STS_TG32, KSEL(SEG_KCODE), vectrap, DPL_KERN);
  set_idt(idt, sizeof(idt));
  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
	_Context *c = (_Context*)stack.end - 1;
	c->eip = (uintptr_t)entry;
	//c->irq = 0x81;
	c->cs = 8;
	
  return c;
}

void _yield() {
  asm volatile("int $0x81");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
