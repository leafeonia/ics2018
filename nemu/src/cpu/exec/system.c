#include "cpu/exec.h"
#include "device/port-io.h"
void difftest_skip_ref();
void difftest_skip_dut();

make_EHelper(lidt) {
	cpu.idtr.base = vaddr_read(id_dest->addr + 2,4);
	cpu.idtr.limit = vaddr_read(id_dest->addr,2);
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  //operand_write(id_dest, &id_src->val);
  assert(id_dest->reg == 0 || id_dest->reg == 3);
  //printf("before mov_r2cr,cpu.eax = %x,cpu.CR0.val = %x,cpu.CR3.val = %x\n",cpu.eax,cpu.CR0.val,cpu.CR3.val);
  //printf("reg_l(%d) = %x\n",id_dest->reg,reg_l(id_dest->reg));
  if(id_dest->reg == 0) cpu.CR0.val = reg_l(id_src->reg);
  else cpu.CR3.val = reg_l(id_src->reg);
  //printf("after mov_r2cr,cpu.eax = %x,cpu.CR0.val = %x,cpu.CR0.PG = %x,cpu.CR3.val = %x,cpu.CR3.base = %x\n",cpu.eax,cpu.CR0.val,cpu.CR0.PG,cpu.CR3.val,cpu.CR3.base);
//printf("id_dest->reg = %d\n",id_dest->reg);
  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
  //printf("---%s---\n",decoding.assembly);
}

make_EHelper(mov_cr2r) {
  //operand_write(id_dest, &id_src->val);
  assert(id_src->reg == 0 || id_src->reg == 3);
  //printf("before mov_cr2r,cpu.eax = %x,cpu.CR0.val = %x,cpu.CR3.val = %x\n",cpu.eax,cpu.CR0.val,cpu.CR3.val);
  if(id_src->reg == 0) rtl_li(&reg_l(id_dest->reg),cpu.CR0.val);
  else rtl_li(&reg_l(id_dest->reg),cpu.CR3.val);
  //printf("after mov_cr2r,cpu.eax = %x,cpu.CR0.val = %x,cpu.CR3.val = %x\n",cpu.eax,cpu.CR0.val,cpu.CR3.val);
//printf("id_src->reg = %d\n",id_src->reg);
  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));
  //printf("---%s---\n",decoding.assembly);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(int) {
  //pr(&id_dest->val);
  raise_intr(id_dest->val,cpu.eip+2);

  print_asm("int %s", id_dest->str);

#if defined(DIFF_TEST) && defined(DIFF_TEST_QEMU)
  difftest_skip_dut();
#endif
}

make_EHelper(iret) {
  rtl_pop(&t0);
  rtl_pop(&cpu.CS);
  rtl_pop(&cpu.eflags.val);
  rtl_j(t0);

  print_asm("iret");
}

make_EHelper(in) {
//pr(&id_src->val);
//pr(&id_src->addr);
  switch(id_src->width){
		case 4:
			rtl_li(&id_dest->val,pio_read_l((ioaddr_t)id_src->val));
			break;
		case 2:
			rtl_li(&id_dest->val,pio_read_w((ioaddr_t)id_src->val));
			break;
		case 1:
		//printf("ret = %d\n",pio_read_b((ioaddr_t)id_src->val));
			rtl_li(&id_dest->val,pio_read_b((ioaddr_t)id_src->val));
			break;
	}
	//pr(&id_dest->val);
  operand_write(id_dest,&id_dest->val);

  print_asm_template2(in);
//printf("---%s---\n",decoding.assembly);
#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(out) {
  //printf("data = %x\n",id_src->val);
  pio_write_b((ioaddr_t)id_dest->val,id_src->val);
	
  print_asm_template2(out);
  //printf("---%s---\n",decoding.assembly);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}
