#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  char *p=(char *)buf;
  for(int i=0;i<len;i++){
  putch(*p++);
  }
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  //printf("getinto\n");
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if (ev.keycode == AM_KEY_NONE) {return 0;}
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
  int len0=sprintf(p,"WIDTH : %d\n HEIGHT : %d\n",t.width,t.height);
  if(len0 <= len) {strcpy(buf,buf0);return len0;}
  else{
  for(int i=0;i<len;i++){
  *(char*)buf++ = *p++;
  }
  return len;
  }
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T t=io_read(AM_GPU_CONFIG);
  //int buf0[6]={0x0000ff,0x0000ff,0x0000ff,0x0000ff,0x0000ff,0x0000ff};
  //int* p=(int*)buf;
  //while(1){printf("%x\n",*p++);}
  int off=offset/4;
  int x=off % t.width;
  int y=off / t.width;
  io_write(AM_GPU_FBDRAW,x,y,(void*)buf,len/4,1,true);
  //io_write(AM_GPU_FBDRAW,x,y,(void*)buf0,6,1,true);
  //printf("write  ok!\n");
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
