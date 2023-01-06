#include <common.h>
#include "syscall.h"
#include <proc.h>
#include <elf.h>
#include <fs.h>
//#include <sys/time.h>
//#define STRACE
int fs_open(const char * ,int ,int);
size_t fs_read(int ,void *, size_t );
size_t fs_write(int ,const void *, size_t);
size_t fs_lseek(int ,size_t, int);
int fs_close(int );
void naive_uload(PCB *, const char *);
void context_uload(PCB* pcb ,const char* filename,char* const argv[],char* const envp[]);
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
#ifdef STRACE
	printf("yield\n");
#endif
}
void sys_exit(Context* c){
	//printf("%d",c->GPR2);
#ifdef STRACE
	printf("exit\n");
#endif
	//halt(c->GPR2);
        c->GPRx = c->GPR2;
}
void sys_write(Context* c){
	/*int fd=c->GPR2;
	const void* buf=(void*)c->GPR3;
	char* p=(char*)buf;
	uint32_t count=c->GPR4;
	if(fd==1 || fd==2){
	for(int i=0;i<count;i++){
	putch(*p);p++;
	}
	c->GPRx=count;}
	else{*/
	c->GPRx=fs_write((int)c->GPR2,(const void*)c->GPR3,(size_t)c->GPR4);//}
#ifdef STRACE
	printf("write\n");
#endif
}
void sys_brk(Context *c){
	//brk(c->GPR2);
	c->GPRx=0;
#ifdef STRACE
	printf("brk\n");
#endif
}
//void sys_brk(Context *);
void sys_open(Context *c){
	c->GPRx=fs_open((char*)c->GPR2,(int)c->GPR3,(int)(c->GPR4));
#ifdef STRACE
	printf("open\n");
#endif
}
void sys_read(Context* c){
	c->GPRx=fs_read((int)c->GPR2,(void*)c->GPR3,(size_t)c->GPR4);
#ifdef STRACE
	printf("read\n");
#endif
}
void sys_close(Context* c){
	c->GPRx=fs_close((int)c->GPR2);
#ifdef STRACE
	printf("close\n");
#endif
}
void sys_lseek(Context* c){
	c->GPRx=fs_lseek((int)c->GPR2,(size_t)c->GPR3,(int)c->GPR4);
#ifdef STRACE
	printf("lseek\n");
#endif
}
void sys_gettimeofday(Context* c){
	//printf("1\n");
	struct timeval{
	uint32_t tv_sec;
	uint32_t tv_usec;
	};
	struct timeval* tv = (struct timeval*)c->GPR2;
	//printf("tv is %p\n",tv);
	uint64_t us=io_read(AM_TIMER_UPTIME).us;
	tv->tv_sec = us / 1000000;
	tv->tv_usec = us % 1000000;
	c->GPRx=0;
	
}
void sys_execve(Context* c){
	printf("execve\n");
	context_uload(current,(const char*)c->GPR2,(char**)c->GPR3,(char**)c->GPR4);
	c->GPRx=-1;
}
void do_syscall(Context *c) {
  switch (c->GPR1) {
    case SYS_exit:sys_exit(c);break;
    case SYS_yield:sys_yield(c);break;
    case SYS_open:sys_open(c);break;
    case SYS_read:sys_read(c);break;
    case SYS_write:sys_write(c);break;
    case SYS_close:sys_close(c);break;
    case SYS_lseek:sys_lseek(c);break;
    case SYS_brk:sys_brk(c);break;
    case SYS_execve:sys_execve(c);break; 
    case SYS_gettimeofday:sys_gettimeofday(c);break;   
    default: panic("Unhandled syscall ID = %d", c->GPR1);
  }
}


