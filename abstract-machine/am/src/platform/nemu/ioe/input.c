#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t val = inw(KBD_ADDR);
  kbd->keycode = val & 0xff;
  kbd->keydown = val & KEYDOWN_MASK;
}
