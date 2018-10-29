#include "cpu/exec.h"
#include "device/port-io.h"
void difftest_skip_ref();
void difftest_skip_dut();

make_EHelper(lidt) {
  TODO();

  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(int) {
  TODO();

  print_asm("int %s", id_dest->str);

#if defined(DIFF_TEST) && defined(DIFF_TEST_QEMU)
  difftest_skip_dut();
#endif
}

make_EHelper(iret) {
  TODO();

  print_asm("iret");
}

make_EHelper(in) {
	pr(&id_src->width);
	/*switch(id_src->width){
		case 4:
			rtl_li(&id_dest->val,pio_read_l(0x3f8));
			break;
		case 2:
			rtl_li(&id_dest->val,pio_read_w(0x3f8));
			break;
		case 1:
			
			break;
	}*/
	rtl_li(&id_dest->val,pio_read_b(0x3f8));
  	pr(&id_dest->val);
  operand_write(id_dest,&id_dest->val);

  print_asm_template2(in);
printf("---%s---\n",decoding.assembly);
#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(out) {
  TODO();

  print_asm_template2(out);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}
