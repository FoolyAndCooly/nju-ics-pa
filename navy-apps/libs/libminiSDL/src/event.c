#include <NDL.h>
#include <SDL.h>

#define keyname(k) #k,
static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};
static int n=sizeof(keyname)/sizeof(char*);
static uint8_t keystate[83]={0};

int SDL_PushEvent(SDL_Event *ev) {
printf("PushEvent not suppote\n");
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  char buf[16]={0};
  char* p=buf+3;
  if(NDL_PollEvent(buf,16) == 0) {return 0;}
  if(*buf=='k'){
  for(int i=0;i<n;i++){
  if(!strcmp(p,keyname[i])){
   ev->key.keysym.sym=i;
  if(*(buf+1)=='d'){ev->type=SDL_KEYDOWN;keystate[i]=1;}
  if(*(buf+1)=='u'){ev->type=SDL_KEYUP;keystate[i]=0;}
  break;}
  }
  return 1;
}
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
  if(numkeys!=NULL){
  *numkeys=0;
  for(int i=0;i<83;i++){
  if(keystate[i]) (*numkeys)++;
  }
  } 
  return keystate;
}
