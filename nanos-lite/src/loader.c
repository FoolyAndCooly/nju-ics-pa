#include <proc.h>
#include <elf.h>
#include <fs.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
size_t ramdisk_read(void* , size_t , size_t );
Context *ucontext(AddrSpace *as, Area kstack, void *entry);
static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  
  Elf_Ehdr ehdr;
  Elf_Phdr phdr;
  uint32_t phoff;
  int fd=fs_open(filename,0,0);
  if(fd==-1)assert(0);
  //ramdisk_read(&ehdr,0,sizeof(Elf_Ehdr));
  fs_read(fd,&ehdr,sizeof(Elf_Ehdr));
  assert(*(uint32_t*)ehdr.e_ident == 0x464c457f);
  int prot=0xe;
  for(int i=0;i<ehdr.e_phnum;i++){
  phoff=i*ehdr.e_phentsize+ehdr.e_phoff;
  //ramdisk_read(&phdr,phoff,sizeof(Elf_Phdr));
  fs_lseek(fd,phoff,SEEK_SET);

  fs_read(fd,&phdr,sizeof(Elf_Phdr));
  
  if(phdr.p_type==PT_LOAD){
  //ramdisk_read((void*)phdr.p_vaddr,phdr.p_offset,phdr.p_filesz);
      uintptr_t va =  ROUNDUP(phdr.p_vaddr,PGSIZE)-PGSIZE;
      uintptr_t va_end = ROUNDUP(phdr.p_vaddr + phdr.p_memsz,PGSIZE);
      int num = ((va_end - va) >> 12);
      void* pa = new_page(num);
      for (int j = 0; j < num; ++ j) {
        map(&pcb->as, (void*)va, (void*)pa,prot);
        va+=PGSIZE;
        pa+=PGSIZE;
      }
      uintptr_t page_off = phdr.p_vaddr & 0xfff; 
      fs_lseek(fd, phdr.p_offset, SEEK_SET);
      fs_read(fd, pa-num*PGSIZE + page_off, phdr.p_filesz); 
  }
  
  }
  //buggy?
  //printf("%x\n",ehdr.e_entry);
  return ehdr.e_entry;
}


void context_uload(PCB* pcb ,const char* filename,char* const argv[],char* const envp[]){
  Area area;
  area.start=pcb->stack;
  area.end=&pcb->stack[STACK_SIZE];
  AddrSpace* adds=&pcb->as;

  protect(adds);
  int prot=0xf;
  char* pa=new_page(8);
  char* va=adds->area.end-8*PGSIZE;
  for(int i=0;i<8;i++){
  map(adds,pa,va,prot);
  pa+=PGSIZE;
  va+=PGSIZE;
  }
  
  void* entry=(void*)loader(pcb,filename);
  pcb->cp=ucontext(adds,area,entry);
  
  char *p=pa;
  char *v=va;
  int argc_count=0,envp_count=0;
  if(argv != NULL){while(argv[argc_count++]!=0);}else{argc_count=1;}
  if(envp != NULL){while(envp[envp_count++]!=0);}else{envp_count=1;}
  int argc=--argc_count,envc=--envp_count;
  char* args[argc],*envs[envc];
  int int_size=sizeof(int);
  int len;
  for(int i=0;i<envc;i++){
  len=strlen(envp[i])+1;
  p-=len;
  v-=len;
  strcpy(p,envp[i]);
  envs[i]=v;
  }
  for(int i=0;i<argc;i++){
  len=strlen(argv[i])+1;
  p-=len;
  v-=len;
  strcpy(p,argv[i]);
  args[i]=v;}
  //printf("send %s to stack\n",args[i]);
 
  v-=int_size;
  p-=int_size;
  *(uint32_t*)p=0;
  //printf("%s\n",args[0]);
  for(int i=envc-1;i>=0;i--){
  p-=int_size;
  v-=int_size;
  *(uint32_t*)p=(uint32_t)envs[i];
  }
  v-=int_size;
  p-=int_size;
  *(uint32_t*)p=0;
  
  for(int i=argc-1;i>=0;i--){
  p-=int_size;
  v-=int_size;
  *(uint32_t*)p=(uint32_t)args[i];
  }

  p-=int_size;
  v-=int_size;
  *(uint32_t*)p=argc;

  pcb->cp->GPRx=(uintptr_t)p;
}
void naive_uload(PCB *pcb, const char *filename) {
  //printf("%s\n",filename);
  //uintptr_t entry = loader(pcb,filename);
  //Log("Jump to entry = %p", entry);
  //((void(*)())entry) ();
}

