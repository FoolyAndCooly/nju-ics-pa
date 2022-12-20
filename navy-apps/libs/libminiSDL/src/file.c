#include <sdl-file.h>

SDL_RWops* SDL_RWFromFile(const char *filename, const char *mode) {
printf("RWFromFile not suppote\n");
  return NULL;
}

SDL_RWops* SDL_RWFromMem(void *mem, int size) {
printf("RWFromMem not suppote\n");
  return NULL;
}
