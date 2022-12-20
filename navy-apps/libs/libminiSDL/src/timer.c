#include <NDL.h>
#include <sdl-timer.h>
#include <stdio.h>

SDL_TimerID SDL_AddTimer(uint32_t interval, SDL_NewTimerCallback callback, void *param) {
printf("AddTimer not suppote\n");
  return NULL;
}

int SDL_RemoveTimer(SDL_TimerID id) {
printf("RemoveTimer not suppote\n");
  return 1;
}

uint32_t SDL_GetTicks() {
  return NDL_GetTicks();
}

void SDL_Delay(uint32_t ms) {
uint32_t time=NDL_GetTicks();
uint32_t now=time;
while(now-time < ms){
	now=NDL_GetTicks();
}
}
