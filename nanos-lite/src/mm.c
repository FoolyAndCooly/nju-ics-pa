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
int prot=0xe;
if(current->max_brk == 0) current->max_brk=brk;//maybe buggy?
if(brk > current->max_brk){
      uintptr_t va = current->max_brk;
      uintptr_t va_end = ROUNDUP(brk,PGSIZE)-PGSIZE;
      int num;
      if(va_end-va >= 0){ num = ((va_end - va) >> 12)+1;}
      else{num=1;assert(0);}
      void* pa = new_page(num);
      for (int j = 0; j < num; ++ j) {
        map(&current->as, (void*)va, (void*)pa,prot);
        va+=PGSIZE;
        pa+=PGSIZE;
      }
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
