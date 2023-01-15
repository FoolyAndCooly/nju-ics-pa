#define concat_temp(x, y) x ## y
#define concat(x, y) concat_temp(x, y)
#define MAP(c, f) c(f)

#if __riscv_xlen == 32
#define LOAD  lw
#define STORE sw
#define XLEN  4
#else
#define LOAD  ld
#define STORE sd
#define XLEN  8
#endif

#define REGS(f) \
      f( 1)       f( 3) f( 4) f( 5) f( 6) f( 7) f( 8) f( 9) \
f(10) f(11) f(12) f(13) f(14) f(15) f(16) f(17) f(18) f(19) \
f(20) f(21) f(22) f(23) f(24) f(25) f(26) f(27)       f(29) \
f(30) f(31)

#define PUSH(n) STORE concat(x, n), (n * XLEN)(sp);
#define POP(n)  LOAD  concat(x, n), (n * XLEN)(sp);

#define KERNEL        0
#define USER          1

#define CONTEXT_SIZE  ((32 + 3 + 1 + 1) * XLEN)
#define OFFSET_SP     ( 2 * XLEN)
#define OFFSET_T3     (28 * XLEN)
#define OFFSET_CAUSE  (32 * XLEN)
#define OFFSET_STATUS (33 * XLEN)
#define OFFSET_EPC    (34 * XLEN)
#define OFFSET_NP     (36 * XLEN)

.align 3
.globl __am_asm_trap
__am_asm_trap:		   
			  
  csrrw sp, mscratch, sp   # if(ksp != 0){
  bnez  sp, maintain_np    #   $sp = ksp;  if !=0 code jump
  csrr  sp, mscratch	   # }

maintain_np:
  addi   sp, sp, -CONTEXT_SIZE
  STORE  t3, OFFSET_T3(sp)     # store t3 first to free it
  csrr   t3, mscratch          
  STORE  t3, OFFSET_SP(sp)     # c->sp = $sp
  
  addi   sp, sp,  CONTEXT_SIZE
  mv     t3, sp
  addi   sp, sp, -CONTEXT_SIZE
  csrw   mscratch, t3
  bnez   t3, L1                # $t3 = (ksp == 0 ? KERNEL : USER)
  li     t3, KERNEL
  jal    change_np  
L1:
  li     t3, USER
  
change_np:
  STORE  t3, OFFSET_NP(sp)     # c->np = $t3
  
  li     t3, 0
  csrw   mscratch, t3          # ksp = 0
  
  
save_context:

  MAP(REGS, PUSH)

  csrr t0, mcause
  csrr t1, mstatus
  csrr t2, mepc
    
  STORE t0, OFFSET_CAUSE(sp)  
  STORE t1, OFFSET_STATUS(sp) 
  STORE t2, OFFSET_EPC(sp)    

  # set mstatus.MPRV to pass difftest
  li a0, (1<<17)
  or t1, t1, a0
  csrw mstatus, t1
  mv a0, sp
  
  jal __am_irq_handle
  
  mv    sp, a0  
  
  LOAD  t3, OFFSET_NP(sp)            # if(c->np == USER){
  bnez  t3, L2                       #   ksp = $sp;
  jal   store_context                # }
L2:
  addi   sp, sp,  CONTEXT_SIZE
  mv     t3, sp
  addi   sp, sp, -CONTEXT_SIZE
  csrw  mscratch ,t3
store_context:
  LOAD  t1, OFFSET_STATUS(sp)
  LOAD  t2, OFFSET_EPC(sp)
  csrw  mstatus, t1
  csrw  mepc, t2
  MAP(REGS,POP)
  LOAD  t3, OFFSET_T3(sp)
  LOAD  sp, OFFSET_SP(sp)          # $sp = c->sp
  
return_from_trap:
  mret
