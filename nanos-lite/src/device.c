#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,
void yield();
int fg_pcb=3;
static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  //yield();
  char *p=(char *)buf;
  for(int i=0;i<len;i++){
  putch(*p++);
  }
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  //printf("getinto\n");
  //yield();
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if (ev.keycode == AM_KEY_NONE) {return 0;}
  switch(ev.keycode){
    case AM_KEY_F1:
      fg_pcb = 1;
      break;
    case AM_KEY_F2:
      fg_pcb = 2;
      break;
    case AM_KEY_F3:
      fg_pcb = 3;
      break;
    }
  //printf("get it !\n");
  char buf0[16]={0};
  char* p=buf0;
  *p++='k';
  if(ev.keydown){*p++='d';}
  else{*p++='u';}
  *p++=' ';
  strcpy(p,keyname[ev.keycode]);
  p +=strlen(keyname[ev.keycode]);
  if (p - buf0 >=len){
  p=buf0;
  for(int i=0;i<len;i++){
  *(char*)buf++ = *p++;
  }
  return len;}
  else{
  strcpy(buf,buf0);
  return p-buf0;
  }
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T t=io_read(AM_GPU_CONFIG);
  char buf0[32]={0};
  char* p=buf0;
  int len0=sprintf(p,"WIDTH : %d\nHEIGHT : %d\n",t.width,t.height);
  if(len0 <= len) {strcpy(buf,buf0);return len0;}
  else{
  for(int i=0;i<len;i++){
  *(char*)buf++ = *p++;
  }
  return len;
  }
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  //yield();
  AM_GPU_CONFIG_T t=io_read(AM_GPU_CONFIG);
  int off=offset/4;
  //printf("%d %d\n",len,off);
  int y=off / t.width;
  int x=off % t.width;
  io_write(AM_GPU_FBDRAW,x,y,(void*)buf,len/4,1,true);
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
