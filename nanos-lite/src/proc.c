#include <proc.h>

#define MAX_NR_PROC 4
void naive_uload(PCB*, const char *);
void context_kload(PCB* pcb,void (*entry)(void *), void *arg);
void context_uload(PCB* pcb ,const char* filename,char* const argv[],char* const envp[]);
static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;
extern int fg_pcb;
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
  char* argv[]={NULL};
  context_uload(&pcb[0], "/bin/hello",argv,NULL);
  context_uload(&pcb[1], "/bin/nslider",argv,NULL);
  context_uload(&pcb[2], "/bin/bird",argv,NULL);
  context_uload(&pcb[3], "/bin/pal",argv,NULL);
  switch_boot_pcb();
  //printf("%s\n",argv[0]);
  //const char* filename="/bin/nterm";
  //naive_uload(NULL, filename);
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
//current = &pcb[0];
current = (current == &pcb[0] ? &pcb[fg_pcb] : &pcb[0]);
// then return the new context
return current->cp;
}
