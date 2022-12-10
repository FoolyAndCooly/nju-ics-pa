#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

//size_t events_read(void *, size_t , size_t );

uint32_t NDL_GetTicks() {
struct timeval{
	uint32_t tv_sec;
	uint32_t tv_usec;
};
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_usec;
}

int NDL_PollEvent(char *buf, int len) {
  int fd=open("/dev/events",0);
  //printf("%d",fd);
  //printf("reach here\n");
  assert(fd);
  int ret=read(fd,(void*)buf,len);
  //printf("%s",buf);
  close(fd);
  return ret;
  //return events_read(buf,0,len);
}

void NDL_OpenCanvas(int *w, int *h) {
  printf("WIDTH : %d\nHEIGHT : %d\n",*w,*h);
  screen_w =*w;
  screen_h =*h;
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
	int fd=open("/dev/fb",0);
	int h0=0,w0=0,y0=y,x0=x;
	//printf("%x",*pixels);
	for(;h0<h;h0++,y0++){
	lseek(fd,screen_w-w,SEEK_CUR);
	for(w0=0,x0=x;w0<w;w0++,x0++){
	write(fd,pixels++,4);
	//printf("%x\n",*pixels);
	}
	}
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  return 0;
}

void NDL_Quit() {
}
