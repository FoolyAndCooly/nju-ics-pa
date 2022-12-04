#include <common.h>
#include "syscall.h"
/*enum {
  SYS_exit,
  SYS_yield,
  SYS_open,
  SYS_read,
  SYS_write,
  SYS_kill,
  SYS_getpid,
  SYS_close,
  SYS_lseek,
  SYS_brk,
  SYS_fstat,
  SYS_time,
  SYS_signal,
  SYS_execve,
  SYS_fork,
  SYS_link,
  SYS_unlink,
  SYS_wait,
  SYS_times,
  SYS_gettimeofday
};*/


void sys_yield(Context *c){
	yield();
	c->GPRx=0;
#ifdef CONFIG_STRACE
	printf("yield\n");
#endif
}
void sys_exit(Context* c){
	//printf("%d",c->GPR2);
	halt(c->GPR2);
#ifdef CONFIG_STRACE
	printf("exit\n");
#endif
}
void sys_write(Context* c){
	int fd=c->GPR2;
	void* buf=(void*)c->GPR3;
	char* p=(char*)buf;
	uint32_t count=c->GPR4;
	if(fd==1 || fd==2){
	for(int i=0;i<count;i++){
	putch(*p);p++;
	}
	c->GPRx=count;
	}
#ifdef CONFIG_STRACE
	printf("write\n");
#endif
}
void sys_brk(Context *c){
	//brk(c->GPR2);
	c->GPRx=0;
}
//void sys_brk(Context *);
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  //printf("%d\n",a[0]);
  switch (a[0]) {
    case SYS_yield:sys_yield(c);break;
    case SYS_exit:sys_exit(c);break;
    case SYS_write:sys_write(c);break;
    case SYS_brk:sys_brk(c);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}


