#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(test) {
  TODO();

  print_asm_template2(test);
}

make_EHelper(and) {
  //TODO();
  rtl_and(&id_dest->val,&id_src->val,&id_src2->val);
    operand_write(id_dest, &id_dest->val);
  print_asm_template2(and);
    printf("---%s---\n",decoding.assembly);
}

make_EHelper(xor) {
  //printf("eax1 = %x\n",cpu.eax);
  rtl_xor(&id_dest->val,&id_src->val,&id_src2->val);
  operand_write(id_dest, &id_dest->val);
  //printf("eax2 = %x\n",cpu.eax);
  print_asm_template2(xor);
  printf("---%s---\n",decoding.assembly);
}

make_EHelper(or) {
  TODO();

  print_asm_template2(or);
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
}

make_EHelper(not) {
  TODO();

  print_asm_template1(not);
}
