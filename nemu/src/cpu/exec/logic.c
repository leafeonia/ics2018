#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(test) {
    rtl_and(&id_dest->val,&id_dest->val,&id_src->val);
    rtl_update_ZFSF(&id_dest->val,id_dest->width);
	rtl_li(&t0,0);
	rtl_set_CF(&t0);
	rtl_set_OF(&t0);
  print_asm_template2(test);
     printf("---%s---\n",decoding.assembly);
}

make_EHelper(and) {
  //TODO();
  //pr(&id_dest->val);pr(&id_src->val);pr(&id_src2->val);
  
  rtl_and(&id_dest->val,&id_dest->val,&id_src->val);
  
  //pr(&id_dest->val);
    operand_write(id_dest, &id_dest->val);
    rtl_update_ZFSF(&id_dest->val,id_dest->width);
    //pr(&id_dest);
  print_asm_template2(and);
    printf("---%s---\n",decoding.assembly);
}

make_EHelper(xor) {
  //printf("eax1 = %x\n",cpu.eax);
  rtl_xor(&id_dest->val,&id_dest->val,&id_src->val);
  operand_write(id_dest, &id_dest->val);
    rtl_update_ZFSF(&id_dest->val,id_dest->width);
  //printf("eax2 = %x\n",cpu.eax);
  print_asm_template2(xor);
  printf("---%s---\n",decoding.assembly);
}

make_EHelper(or) {
  rtl_or(&id_dest->val,&id_dest->val,&id_src->val);
  operand_write(id_dest, &id_dest->val);
    rtl_update_ZFSF(&id_dest->val,id_dest->width);

  print_asm_template2(or);
  printf("---%s---\n",decoding.assembly);
}

make_EHelper(sar) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
    printf("---%s---\n",decoding.assembly);
}

make_EHelper(not) {
  TODO();

  print_asm_template1(not);
}
