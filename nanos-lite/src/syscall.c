#include <common.h>
#include "syscall.h"
int fs_open(const char * ,int ,int);
size_t fs_read(int ,void *, size_t );
size_t fs_write(int ,const void *, size_t);
size_t fs_lseek(int ,size_t, int);
int fs_close(int );
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
	printf("exit\n");
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
void sys_open(Context *c){
	printf("open\n");
	c->GPRx=fs_open((char*)c->GPR2,(int)c->GPR3,(int)(c->GPR4));
}
void sys_read(Context* c){
	c->GPRx=fs_read((int)c->GPR2,(void*)c->GPR3,(size_t)c->GPR4);
}
void sys_close(Context* c){
	c->GPRx=fs_close((int)c->GPR2);
}
void sys_lseek(Context* c){
	c->GPRx=fs_lseek((int)c->GPR2,(size_t)c->GPR3,(int)c->GPR4);
}
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  //printf("%d\n",a[0]);
  switch (a[0]) {
    case SYS_exit:sys_exit(c);break;
    case SYS_yield:sys_yield(c);break;
    case SYS_open:sys_open(c);break;
    case SYS_read:sys_read(c);break;
    case SYS_write:sys_write(c);break;
    case SYS_close:sys_close(c);break;
    case SYS_lseek:sys_lseek(c);break;
    case SYS_brk:sys_brk(c);break;
    
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}


