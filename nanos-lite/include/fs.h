#ifndef __FS_H__
#define __FS_H__

#include "common.h"
int fs_open(const char* pathname);
size_t fs_filesz(int fd);
size_t fs_read(int fd, void* buf, size_t len);
int fs_close(int fd);
#ifndef SEEK_SET
enum {SEEK_SET, SEEK_CUR, SEEK_END};
#endif

#endif
