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
void context_uload(PCB* pcb ,char* filename){
  Area area;
  area.start=pcb->stack;
  area.end=&pcb->stack[STACK_SIZE];
  void* entry=(void*)loader(pcb,filename);
  pcb->cp=ucontext(NULL,area,entry);
}
void naive_uload(PCB *pcb, const char *filename) {
  //printf("%s\n",filename);
  uintptr_t entry = loader(pcb,filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

