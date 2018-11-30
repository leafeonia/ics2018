#include "common.h"
#include <amdev.h>

size_t ramdisk_write(const void *buf, size_t offset, size_t len);

size_t serial_write(const void *buf, size_t offset, size_t len) {
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
  return 0;
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  memcpy(buf,dispinfo+offset,len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  draw_rect((uint32_t*)buf,offset/4%screen_width(),offset/4%screen_height(),len/4,1);	
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();
  sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d\n",screen_width(),screen_height());
  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
