#ifndef __ARCH_H__
#define __ARCH_H__

#include <am.h>

#define PMEM_SIZE (128 * 1024 * 1024)
#define PGSIZE    4096    // Bytes mapped by a page

typedef uint32_t size_t;

struct _Context {
  struct _Protect *prot;
  uintptr_t edi,esi,ebp,esp,ebx,edx,ecx,eax;
  int       irq;
  uintptr_t err, eip, cs, eflags;
  /*
  uintptr_t eflags, cs, eip, err;
  int       irq;
  struct _Protect *prot;
  uintptr_t eax, ecx, edx, ebx, esp, ebp, esi, edi;*/
};

#define GPR1 err
#define GPR2 irq
#define GPR3 ebp
#define GPR4 esi
#define GPRx eip

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
