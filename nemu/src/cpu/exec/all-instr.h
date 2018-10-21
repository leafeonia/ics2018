#include "cpu/exec.h"

make_EHelper(mov);
make_EHelper(call);
make_EHelper(push);
make_EHelper(sub); //eflags
make_EHelper(xor); //eflags
make_EHelper(pop);
make_EHelper(ret);
make_EHelper(lea);
make_EHelper(and); //eflags
make_EHelper(nop);

make_EHelper(operand_size);

make_EHelper(inv);
make_EHelper(nemu_trap);
