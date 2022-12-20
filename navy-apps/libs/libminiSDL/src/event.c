#include <NDL.h>
#include <SDL.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
printf("PushEvent not suppote\n");
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  char* buf=(char*)malloc(sizeof(char)*10);
  char* p=buf+3;
  int n=sizeof(keyname)/sizeof(char*);
  if(NDL_PollEvent(buf,16) == 0) return 0;
  if(*buf=='k'){
  if(*(buf+1)=='d')ev->type=SDL_KEYDOWN;
  if(*(buf+1)=='u')ev->type=SDL_KEYUP;
  for(int i=0;i<n;i++){
  if(strcmp(p,keyname[i])){ ev->key.keysym.sym=i;break;}}
  }
  return 1;
}

int SDL_WaitEvent(SDL_Event *event) {
  while(!SDL_PollEvent(event)) ;
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
printf("PeepEvent not suppote\n");
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
printf("GetKeyState not suppote\n");
  return NULL;
}
