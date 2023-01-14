#include <memory.h>
#include <proc.h>
static void *pf = NULL;

void* new_page(size_t nr_page) {
  void* pre_pf=pf;
  pf+=nr_page * PGSIZE;
  assert(pf<heap.end);
  return pre_pf;
}

#ifdef HAS_VME
static void* pg_alloc(int n) {
  void* p=new_page(n/PGSIZE);
  memset(p,0,n);
  return p;
}
#endif

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
//printf("brk %x %x\n",current->max_brk,brk);
  int prot = 0xe;
  uintptr_t va=current->max_brk;
  void* pa=NULL;
  while(va<ROUNDUP(brk ,PGSIZE)){
  pa=new_page(1);
  map(&current->as,(void*)va,pa,prot);
  va+=PGSIZE;
  }//it is different from load ,so we shouldnot copy from it
  if(brk > current->max_brk) current->max_brk=brk;
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);
#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}
