#include <common.h>
#include "syscall.h"
void do_syscall(Context *c);
static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_YIELD:
    printf("trapped\n");
    c->mepc +=4;break;
    case EVENT_SYSCALL:
    do_syscall(c);
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
