#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  uint32_t addr = cpu.idtr.base + NO*8;
  printf("addr = %d\n",addr);
  uint32_t low = vaddr_read(addr,4);
  uint32_t high = vaddr_read(addr+12,4);
  uint32_t jp = (high << 16) & low;
  printf("jp = %d\n",jp);
  cpu.eip = jp;
  cpu.eip = ret_addr;
}

void dev_raise_intr() {
}
