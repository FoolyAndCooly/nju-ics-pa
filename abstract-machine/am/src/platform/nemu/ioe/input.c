#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  kbd->keydown = inl(RTC_ADDR) & KEYDOWN_MASK;
  kbd->keycode = inb(RTC_ADDR);
}
