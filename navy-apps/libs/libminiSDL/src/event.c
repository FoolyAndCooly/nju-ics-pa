#include <NDL.h>
#include <SDL.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  char* buf=(char*)malloc(sizeof(char)*10);
  char* p=buf+3;
  if(NDL_PollEvent(buf,16) == 0) return 0;
  if(*buf=='k' && *(buf+1)=='d'){
  ev->type=SDL_KEYDOWN;
  if(strcmp(p,"DOWN")){ ev->key.keysym.sym=SDLK_DOWN;printf("DOWN\n");}
  if(strcmp(p,"UP")){ ev->key.keysym.sym=SDLK_UP; printf("UP\n");}
  }
  return 1;
}

int SDL_WaitEvent(SDL_Event *event) {
  while(!SDL_PollEvent(event)) ;
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
