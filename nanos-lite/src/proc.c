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
  //switch_boot_pcb();
  //const char* filename="/bin/nterm";
  //naive_uload(NULL, filename);
  char* str="/bin/exec";
  printf("%p\n%s\n",str,str);
  char* argv[2]={str,NULL};
  printf("%p\n",argv[1]);
  Log("Initializing processes...");

  // load program here

}

Context* schedule(Context *prev) {
  return NULL;
}
