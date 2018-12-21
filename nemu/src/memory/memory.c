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
    //printf("base = %x\n",cpu.CR3.base);
	paddr_t dire_addr = (cpu.CR3.base<<12) | (4*PDX(vaddr));
	PDE table_index;
	table_index.val = paddr_read(dire_addr,4);
	if(table_index.present == 0){
		printf("\033[31m\033[1m*pde = 0x%x, pde = 0x%x\n\033[0m",dire_addr,table_index.val);
	}
	assert(table_index.present == 1);
	paddr_t addr2 = (table_index.page_frame << 12) | (4*PTX(vaddr));
	PTE table;
	table.val = paddr_read(addr2,4);
	assert(table.present == 1);
	return ((table.page_frame << 12) | OFF(vaddr));
}

uint32_t vaddr_read(vaddr_t addr, int len) {
  if(cpu.CR0.PG == 1){
  	uint32_t last = (addr & 0xfff);
	if (last + len - 1 > 0xfff) 
	{
		uint32_t distance = 0xfff - last; 
		uint32_t differ = len - 1 - distance; 
		paddr_t addrtemp = addr + distance + 1;
		uint32_t hwtemp = page_translate(addrtemp);
		uint32_t right = paddr_read(hwtemp,differ);
		uint32_t left = paddr_read(page_translate(addr), distance + 1);
		right <<= (distance + 1) * 8;
		return right + left;
	}
  	
  	else{
  		return paddr_read(page_translate(addr),len);
  	}
  }
  else return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if(cpu.CR0.PG == 1){
  	uint32_t last = (addr & 0xfff);
  	if(last + len - 1 > 0xfff) 
		{
			uint32_t distance = 0xfff - last;
			uint32_t differ = len - 1 - distance;
			paddr_t addrtemp = addr + distance + 1;
			paddr_write(addrtemp, differ, (data >> ((distance + 1) * 8)));
			paddr_write(addr, distance + 1, data);
		}
  	else{
  		return paddr_write(page_translate(addr),data,len);
  	}
  }
  else paddr_write(addr, data, len);
}
