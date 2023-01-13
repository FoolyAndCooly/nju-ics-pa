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
#include <memory/vaddr.h>
paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {
  uint32_t vpn1=vaddr>>22,vpn0=vaddr>>12 & 0x3ff;
  uint32_t offset=vaddr & 0xfff;
  uint32_t PTESIZE=4;
  paddr_t a=cpu.csr.satp<<12;
  printf("%x\n",a);
  uint32_t pte=paddr_read(a+vpn1*PTESIZE,PTESIZE);
  uint32_t leaf_pte=paddr_read(pte+vpn0*PTESIZE,PTESIZE);
  assert(leaf_pte & 1);
  paddr_t paddr = (pte & ~0xfff) | offset;
  assert(vaddr==paddr);
  return paddr;
}
