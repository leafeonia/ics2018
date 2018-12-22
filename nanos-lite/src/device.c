#include "common.h"
#include <amdev.h>

size_t ramdisk_write(const void *buf, size_t offset, size_t len);

size_t serial_write(const void *buf, size_t offset, size_t len) {
//	_yield();
	int ret = len;
	while(len--) _putc(*(char*)buf++);
  	return ret;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
//	_yield();
  int key = read_key();
  int down = 0;
  if(key & 0x8000) down = 1;
  else down = 0;
  key &= 0x7fff;
  if(key != _KEY_NONE){
    //printf("keyname[%d] = %s\n",key,keyname[key]);
  	if(down) sprintf(buf,"kd %s\n",keyname[key]);
  	else sprintf(buf,"ku %s\n",keyname[key]);
  }
  else{
  	int timee = uptime();
  	sprintf(buf,"t %d\n",timee);
  }
  
  return (strlen(buf) <= len ? strlen(buf) : len);
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
 // printf("enter dispinfo_read\n");
  memcpy(buf,dispinfo+offset,len);
  return strlen(buf);
}


size_t fb_write(const void *buf, size_t offset, size_t len) {
//	_yield();
  //printf("enter fb_write\n");
  draw_rect((uint32_t*)buf,offset/4%screen_width(),offset/4/screen_width(),len/4,1);	
  return len;
}



void init_device() {
  Log("Initializing devices...");
  _ioe_init();
  sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d\n",screen_width(),screen_height());
  
  //printf("dispinfo:%s\n",dispinfo);
  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
