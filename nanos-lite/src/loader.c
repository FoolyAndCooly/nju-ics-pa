#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
size_t ramdisk_read(void* , size_t , size_t );
static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  Elf_Ehdr ehdr;
  Elf_Phdr phdr;
  uint32_t phoff;
  ramdisk_read(&ehdr,0,sizeof(Elf_Ehdr));
  for(int i=0;i<ehdr.e_phnum;i++){
  phoff=i*ehdr.e_phentsize+ehdr.e_phoff;
  ramdisk_read(&phdr,phoff,sizeof(Elf_Phdr));
  if(phdr.p_type==PT_LOAD){
  ramdisk_read((void*)phdr.p_vaddr,phdr.p_offset,phdr.p_filesz);
  memset((void*)phdr.p_vaddr,0,phdr.p_memsz - phdr.p_filesz);
  }
  }
  return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

