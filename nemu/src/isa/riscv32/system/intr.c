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
#define IRQ_TIMER 0x80000007
word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  cpu.csr.mepc = epc;
  cpu.csr.mcause = NO;
  if ((cpu.csr.mstatus & 0x8) != 0) {cpu.csr.mstatus |= 0x80;}
  else{cpu.csr.mstatus &= ~0x80;}
  cpu.csr.mstatus &= ~0x8;
  return cpu.csr.mtvec;
}

word_t isa_query_intr() {
 if (cpu.INTR == true && ((cpu.csr.mstatus & 0x8) != 0)) {
    cpu.INTR = false;
    return IRQ_TIMER;
  }
  return INTR_EMPTY;
}
