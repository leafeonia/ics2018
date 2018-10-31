#include <am.h>
#include <x86.h>
#include <amdev.h>

size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
    _putc('1');
      int code  = inl(0x60);
      if(code) _putc(code);
      else _putc(0);
      _KbdReg *kbd = (_KbdReg *)buf;
      kbd->keydown = ((code >> 15) & 1);
      kbd->keycode = code & 0x7fff;
      return sizeof(_KbdReg);
    }
  }
  return 0;
}
