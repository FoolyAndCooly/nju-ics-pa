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
#include <cpu/difftest.h>
#include "../local-include/reg.h"
const char *reg[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};


bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  if(cpu.pc != ref_r->pc){
	printf("pc is wrong\nref pc : 0x%08x\ncpu pc : 0x%08x\n",ref_r->pc,cpu.pc );
	}
	for(int i=0;i<32;i++){
		if (cpu.gpr[i] !=ref_r->gpr[i]){
		printf("%s reg is wrong\nref reg :0x%08x\ncpu reg :0x%08x\n",reg[i],ref_r->gpr[i],cpu.gpr[i]);
		return false ;
		}
	}
  if(cpu.csr.mepc != ref_r->csr.mepc){printf("mepc is wrong\nref mepc :0x%08x\ncpu mepc :0x%08x\n",ref_r->csr.mepc,cpu.csr.mepc);return false;}
  if(cpu.csr.mstatus != ref_r->csr.mstatus){printf("mstatus is wrong\nref mstatus :0x%08x\ncpu mstatus :0x%08x\n",ref_r->csr.mstatus,cpu.csr.mstatus);return false;}
  if(cpu.csr.mcause != ref_r->csr.mcause){printf("mcause is wrong\nref mcause :0x%08x\ncpu mcause :0x%08x\n",ref_r->csr.mcause,cpu.csr.mcause);return false;}
  if(cpu.csr.mtvec != ref_r->csr.mtvec){printf("mtvec is wrong\nref mtvec :0x%08x\ncpu mtvec :0x%08x\n",ref_r->csr.mtvec,cpu.csr.mtvec);return false;}
	return true;
}

void isa_difftest_attach() {
}
