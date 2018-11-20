#include "common.h"
#include "syscall.h"

#define SYS_yield 1 //add

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  switch (a[0]) {
    case(SYS_yield):_yield();
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
