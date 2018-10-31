#include <am.h>
#include <x86.h>
#include <amdev.h>
#include <klib.h>

static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000;

size_t video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _VideoInfoReg *info = (_VideoInfoReg *)buf;
      info->width = 400;
      info->height = 300;
      return sizeof(_VideoInfoReg);
    }
  }
  return 0;
}

size_t video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _FBCtlReg *ctl = (_FBCtlReg *)buf;
      
      int i,j;
      for(i = ctl->x;i < ctl->x + ctl->w;++i){
	  	for(j = ctl->y;j < ctl->y + ctl->h;++i){
	  		fb[i*screen_width()+j] = *(ctl->pixels);
	  	}
	  }

      int size = screen_width() * screen_height();
      for (i = 0; i < size; i ++) fb[i] = i;

		
	  

      if (ctl->sync) {
        // do nothing, hardware syncs.
      }
      return sizeof(_FBCtlReg);
    }
  }
  return 0;
}

void vga_init() {
}
