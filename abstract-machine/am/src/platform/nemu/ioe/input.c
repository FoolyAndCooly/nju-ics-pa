#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
kbd->keydown = 0;
  kbd->keycode = AM_KEY_NONE;
  uint32_t state = *(volatile uint32_t *)(KBD_ADDR);
  // decode出keycode
  kbd->keycode = state & ~KEYDOWN_MASK;
  if ((state & KEYDOWN_MASK) != 0) {
    kbd->keydown = true;
  }
  //kbd->keydown = inl(KBD_ADDR) & KEYDOWN_MASK;
  //kbd->keycode = inl(KBD_ADDR);
}
