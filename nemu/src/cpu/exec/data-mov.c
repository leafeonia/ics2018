#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
  printf("---%s---\n",decoding.assembly);
}

make_EHelper(push) {
pr(&id_dest->val);
  rtl_push(&id_dest->val);
   if(id_dest->type != OP_TYPE_IMM)operand_write(id_dest, &id_dest->val);
  print_asm_template1(push);
  printf("---%s---\n",decoding.assembly);
}

make_EHelper(pop) {
	//pr(&id_dest->val);
  rtl_pop(&id_dest->val);
   if(id_dest->type != OP_TYPE_IMM)operand_write(id_dest, &id_dest->val);
  print_asm_template1(pop);
  printf("---%s---\n",decoding.assembly);
}

make_EHelper(pusha) {


  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  rtl_mv(&reg_l(R_ESP),&reg_l(R_EBP));
  rtl_pop(&reg_l(R_EBP));
  print_asm("leave");
  printf("---%s---\n",decoding.assembly);
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t0, &id_src->val, id_src->width);
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
      printf("---%s---\n",decoding.assembly);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
    printf("---%s---\n",decoding.assembly);
}
