#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  uint32_t addr = cpu.idtr.base + NO*8;
  printf("addr = %d\n",addr);
  uint32_t low = vaddr_read(addr,2);
  printf("low = %d\n",low);
  uint32_t high = vaddr_read(addr+6,2);
  printf("high = %d\n",high);
  uint32_t jp = (high << 16) | low;
  printf("jp = %d\n",jp);
  rtl_push(&ret_addr);
  rtl_j(jp);
 // cpu.eip = ret_addr;
}

void dev_raise_intr() {
}
