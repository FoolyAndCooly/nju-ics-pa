#include <proc.h>

#define MAX_NR_PROC 4
void naive_uload(PCB*, const char *);
static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void init_proc() {
  switch_boot_pcb();
  const char* filename="/bin/nterm";
  naive_uload(NULL, filename);
  Log("Initializing processes...");

  // load program here

}
void context_kload(PCB* pcb,void (*entry)(void *), void *arg){
  Area area;
  area.start=pcb->stack;
  area.end=&pcb->stack[STACK_SIZE];
  pcb->cp=kcontext(area,entry,arg); 
}

Context* schedule(Context *prev) {
// save the context pointer
current->cp = prev;

// always select pcb[0] as the new process
current = &pcb[0];

// then return the new context
return current->cp;
}
