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
  int i=ctl->y, j, h0=0, w0;
  uint32_t val = inl(VGACTL_ADDR);
  uint32_t width = val >> 16;
  for (; h0 < ctl->h; ++ i) {
  	w0=0;
    for (j=ctl->x; w0 < ctl->w; ++ j) {
      outl(FB_ADDR + (i * width + j) * 4, *((int *)ctl->pixels + h0* ctl->w + w0));
      w0++;
    }
    h0++;
  }
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
