#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
	int i;
	int w=400;
	int h=300;
	uint32_t *fb=(uint32_t*)(uintptr_t)FB_ADDR;
	for(i=0;i<w*h;i++)fb[i]=i;
	outl(SYNC_ADDR,1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t val=inl(VGACTL_ADDR);
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = val >> 16, .height = val & 0xffff,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
int i, j, pi, pj;
  uint32_t* pixels = ctl->pixels;
  uint32_t wh_data = inl(VGACTL_ADDR);
  uint32_t width = (wh_data >> 16) & 0xffff;
  for (i = ctl->y, pi = 0; pi < ctl->h; ++ i, ++pi) {
    for (j = ctl->x, pj = 0; pj < ctl->w; ++ j, ++pj) {
      int fb_offset = (i * width + j) * 4;
      int pixel_offset = pi* ctl->w + pj;
      outl(FB_ADDR + fb_offset, *(pixels + pixel_offset));
    }
  }
  /*uint32_t val=inl(VGACTL_ADDR);
  int i=ctl->x,j=ctl->y,w0=0,h0=0;
  uint32_t width=val>>16;
  for(;i<ctl->y + ctl->h;i++){
  	for(;j<ctl->x + ctl->w;j++){
  	outl(FB_ADDR + (i * width +j)*4,*((uint32_t*)ctl->pixels+w0*ctl->w+h0));
  	h0++;
  	}
  	w0++;
  }*/
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
