#include "nemu.h"
#include "device/mmio.h"
#include "memory/mmu.h"
#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
//if(addr > 0x110000)pr(&addr);
  if(is_mmio(addr) != -1){
  	return mmio_read(addr,len,is_mmio(addr));
  }
  else return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
}

void paddr_write(paddr_t addr, uint32_t data, int len) {
  if(is_mmio(addr) != -1){
  	mmio_write(addr,len,data,is_mmio(addr));
  }
  else memcpy(guest_to_host(addr), &data, len);
}

paddr_t page_translate(vaddr_t vaddr){
	paddr_t dire_addr = (cpu.CR3.base<<12) | (4*PDX(vaddr));
	PDE table_index;
	table_index.val = paddr_read(dire_addr,4);
	assert(table_index.present == 1);
	paddr_t addr2 = (table_index.page_frame << 12) | (4*PTX(vaddr));
	PTE table;
	table.val = paddr_read(addr2,4);
	assert(table.present == 1);
	return ((table.page_frame << 12) | OFF(vaddr));
}

uint32_t vaddr_read(vaddr_t addr, int len) {
  /*if(cpu.CR0.PG == 1){
  	if(((addr+len)&0xfffff000) != (addr&0xfffff000)){
  		uint32_t len2 = (addr+len) & 0xfff;
  		uint32_t len1 = len - len2;
  		uint32_t addr2=(addr+len)&0xfffff000;
		paddr_t paddr1=page_translate(addr);
		paddr_t paddr2=page_translate(addr2);
		uint32_t res_1=paddr_read(paddr1,len1);
		uint32_t res_2=paddr_read(paddr2,len2);
		res_1=((res_1<<(len2*8))>>(len2*8));
		res_2=(res_2<<(len1*8));
		return (res_1 | res_2);
  		//panic("THE FUCKING ADDR = %x\n",addr);
  		//assert(0);
  	}
  	else{
  		return paddr_read(page_translate(addr),len);
  	}
  }
  else */return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, uint32_t data, int len) {
  paddr_write(addr, data, len);
}
