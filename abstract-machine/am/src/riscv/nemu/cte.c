#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

#define KERNEL 1
#define USER 0
static Context* (*user_handler)(Event, Context*) = NULL;
void __am_get_cur_as(Context *c);
void __am_switch(Context *c);

void display_context(Context *c,bool flag)
{
  if(flag) printf("into cte\n");
  else printf("out of cte\n");
  for (int i = 0; i < sizeof(c->gpr) / sizeof(c->gpr[0]); i++)
  printf("gpr %d is %x\n", i, c->gpr[i]);
  printf("c->np %d\n",c->np);
  printf("mcause, mstatus, mepc is %x, %x, %x\n\n", c->mcause, c->mstatus, c->mepc);
}

Context* __am_irq_handle(Context *c) {
  __am_get_cur_as(c);
  display_context(c,1);
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case 0xb:
      if ((int)c->GPR1 >= 0 ) {
          ev.event = EVENT_SYSCALL;
      }else{
      ev.event = EVENT_YIELD;
      }
      break;
      case 0X80000007:
        ev.event = EVENT_IRQ_TIMER;
        //printf("EVENT_IRQ_TIMER\n");
        break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }
  __am_switch(c);
  display_context(c,0);
  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  Context* context=kstack.end-sizeof(Context);
  context->mepc = (uintptr_t)entry;
  context->mstatus = 0x1880;
  context->mcause = 0xb;
  context->GPRx=(uintptr_t)arg;
  context->pdir=NULL;
  context->gpr[2] =(uintptr_t)context+sizeof(Context);
  context->np =0 ;
  return context;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
