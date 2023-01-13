#include <memory.h>
#include <proc.h>
static void *pf = NULL;

void* new_page(size_t nr_page) {
  void* pre_pf=pf;
  pf+=nr_page * PGSIZE;
  memset(pre_pf,0,nr_page*PGSIZE);
  return pre_pf;
}

#ifdef HAS_VME
static void* pg_alloc(int n) {
  void* p=new_page(n/PGSIZE);
  return p;
}
#endif

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
printf("%x %x\n",current->max_brk,brk);
 if (current->max_brk == 0) {
    current->max_brk = brk;
    return 0;
  }
  int prot = 0xe;
  for (; current->max_brk <= ROUNDUP(brk, PGSIZE); current->max_brk += PGSIZE) {
    void* pa = pg_alloc(PGSIZE);
    map(&current->as, (void*)current->max_brk, pa, prot);
  }
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);
#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}
