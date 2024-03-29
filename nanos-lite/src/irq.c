#include "common.h"

extern _Context* do_syscall(_Context *c);
extern _Context* schedule(_Context *prev);//add

static _Context* do_event(_Event e, _Context* c) {
  switch (e.event) {
	case _EVENT_YIELD:
		//printf("_EVENT_YIELD\n");
	//	break;
		//printf("irq.c: schedule(c) returns %x,c->eip = %x \n",schedule(c),schedule(c)->eip);
		return schedule(c);
	case _EVENT_SYSCALL:
		do_syscall(c);
		break;
	case _EVENT_IRQ_TIMER:
		//Log("_EVENT_IRQ_TIMER");
		_yield();
		break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return NULL;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
