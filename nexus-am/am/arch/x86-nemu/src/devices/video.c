#include <am.h>
#include <x86.h>
#include <amdev.h>
#include <klib.h>

static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000;

size_t video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
    int ret = inl(0x100);
      _VideoInfoReg *info = (_VideoInfoReg *)buf;
      info->width = ret >> 16;
      info->height = ret & 0xffff;
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
      //int size = screen_width() ;* screen_height();
      //for (i = 0; i < size; i ++) fb[i] = i;
      
      /*int s = ctl->w*ctl->h;
	  for(i = 0;i < s;++i){
	  	fb[i+ctl->x*screen_width()+ctl->y] = *(ctl->pixels+i);
	  	
	  }*/
	  int cnt = 0;
	  for(i = 0;i < ctl->h;i++){
	  	for(j = 0;j < ctl->w;j++){
	  		fb[ctl->x+j + (ctl->y+i)*screen_width()] = *(ctl->pixels+cnt);
	  		cnt++;
	  	}
	  }
		
	  /*for(i = ctl->x;i < ctl->x + ctl->w;++i){
	  	for(j = ctl->y;j < ctl->y + ctl->h;++j){
	  		fb[i*screen_width()+j] = *(ctl->pixels+i*ctl->w+j);
	  	}
	  }*/

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
