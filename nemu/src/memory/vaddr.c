/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <memory/paddr.h>
paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type);
word_t vaddr_ifetch(vaddr_t addr, int len) {
  int check=isa_mmu_check(addr,len,MEM_TYPE_IFETCH),ret;
  if (check == MMU_DIRECT) {ret=paddr_read(addr, len);}
  else {ret = paddr_read(isa_mmu_translate(addr,len,MEM_TYPE_IFETCH),len);}
  return ret;
}

word_t vaddr_read(vaddr_t addr, int len) {
  int check=isa_mmu_check(addr,len,MEM_TYPE_READ),ret;
  if (check == MMU_DIRECT) {ret=paddr_read(addr, len);}
  else {ret = paddr_read(isa_mmu_translate(addr,len,MEM_TYPE_READ),len);}
  return ret;
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
  int check=isa_mmu_check(addr,len,MEM_TYPE_WRITE);
  if (check == MMU_DIRECT) {paddr_write(addr, len,data);}
  else {paddr_write(isa_mmu_translate(addr,len,MEM_TYPE_WRITE),len,data);}
}
