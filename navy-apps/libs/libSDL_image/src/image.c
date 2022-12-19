#define SDL_malloc  malloc
#define SDL_free    free
#define SDL_realloc realloc

#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"
#include <assert.h>
SDL_Surface* STBIMG_LoadFromMemory(const unsigned char*,int);

SDL_Surface* IMG_Load_RW(SDL_RWops *src, int freesrc) {
  assert(src->type == RW_TYPE_MEM);
  assert(freesrc == 0);
  return NULL;
}

SDL_Surface* IMG_Load(const char *filename) {
  FILE* fp=fopen(filename,"r+");
  assert(fp);
  fseek(fp,0,SEEK_END);
  uint32_t size=ftell(fp);
  fseek(fp,0,SEEK_SET);
  char *buf=(char*)malloc(size*sizeof(char));
  memcpy(buf,fp,size);
  SDL_Surface* sdl=STBIMG_LoadFromMemory(buf,size);
  fclose(fp);
  free(buf);
  return sdl;
}

int IMG_isPNG(SDL_RWops *src) {
  return 0;
}

SDL_Surface* IMG_LoadJPG_RW(SDL_RWops *src) {
  return IMG_Load_RW(src, 0);
}

char *IMG_GetError() {
  return "Navy does not support IMG_GetError()";
}
