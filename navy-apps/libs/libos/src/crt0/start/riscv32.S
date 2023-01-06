.globl  _start
_start:
  move sp, a0
  move s0, zero
  jal call_main
