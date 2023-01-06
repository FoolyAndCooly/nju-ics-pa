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
  for(int i=0;i<ehdr.e_phnum;i++){
  phoff=i*ehdr.e_phentsize+ehdr.e_phoff;
  //ramdisk_read(&phdr,phoff,sizeof(Elf_Phdr));
  fs_lseek(fd,phoff,SEEK_SET);
  fs_read(fd,&phdr,sizeof(Elf_Phdr));
  if(phdr.p_type==PT_LOAD){
  //ramdisk_read((void*)phdr.p_vaddr,phdr.p_offset,phdr.p_filesz);
  fs_lseek(fd,phdr.p_offset,SEEK_SET);
  fs_read(fd,(void*)phdr.p_vaddr,phdr.p_filesz);
  memset((void*)(phdr.p_vaddr+phdr.p_filesz),0,phdr.p_memsz - phdr.p_filesz);
  }
  } //buggy?
  //printf("%x\n",ehdr.e_entry);
  return ehdr.e_entry;
}

char* set_NULL(char* p){
  int int_size=sizeof(int);
  p-=int_size;
  *(uint32_t*)p=0;
  return p;
}

void context_uload(PCB* pcb ,const char* filename,char* const argv[],char* const envp[]){
  Area area;
  area.start=pcb->stack;
  area.end=&pcb->stack[STACK_SIZE];
  void* entry=(void*)loader(pcb,filename);
  pcb->cp=ucontext(NULL,area,entry);
  char* p=new_page(8);
  int argc_count=0,envp_count=0;
  if(argv != NULL){while(argv[argc_count++]);}else{argc_count=1;}
  if(envp != NULL){while(envp[envp_count++]);}else{envp_count=1;}
  int argc=--argc_count,envc=--envp_count;
  char* args[argc],*envs[envc];
  int int_size=sizeof(int);
  for(int i=0;i<envc;i++){
  p-=strlen(envp[i])+1;
  strcpy(p,envp[i]);
  envs[i]=p;
  }
  printf("%d\n",argc);
  for(int i=0;i<argc;i++){
  p-=strlen(argv[i])+1;
  strcpy(p,argv[i]);
  args[i]=p;
  //printf("send %s to stack\n",args[i]);
  }
  
  p=set_NULL(p);
  //printf("%s\n",args[0]);
  for(int i=envc-1;i>=0;i--){
  p-=int_size;
  *(uint32_t*)p=(uint32_t)envs[i];
  }
  p=set_NULL(p);
  
  for(int i=argc-1;i>=0;i--){
  p-=int_size;
  *(uint32_t*)p=(uint32_t)args[i];
  }

  p-=int_size;
  *(uint32_t*)p=argc;
  pcb->cp->GPRx=(uintptr_t)p;
}
void naive_uload(PCB *pcb, const char *filename) {
  //printf("%s\n",filename);
  uintptr_t entry = loader(pcb,filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

