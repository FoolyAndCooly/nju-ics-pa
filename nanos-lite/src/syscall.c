#include <common.h>
#include "syscall.h"
#define SYS_yield 1
#define SYS_exit 0
void sys_yield(Context *c){
	yield();
	c->GPRx=0;
#ifdef CONFIG_STRACE
	printf("yield");
#endif
}
void sys_exit(Context* c){
	halt(c->GPR2);
#ifdef CONFIG_STRACE
	printf("exit"));
#endif
}
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case SYS_yield:sys_yield(c);break;
    case SYS_exit:sys_exit(c);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

