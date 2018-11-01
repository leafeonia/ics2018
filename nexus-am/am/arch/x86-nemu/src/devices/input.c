#include <am.h>
#include <x86.h>
#include <amdev.h>

size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {

      //int code  = inl(0x60);
      _KbdReg *kbd = (_KbdReg *)buf;
      //kbd->keydown = ((code >> 15) & 1);
      //kbd->keycode = code & 0x7fff;
      int32_t k = inl(0x60);
      int32_t s = inb(0x64);
      kbd->keydown = s;
      kbd->keycode = k;
      return sizeof(_KbdReg);
    }
  }
  return 0;
}
