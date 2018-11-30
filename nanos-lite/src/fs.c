#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0,invalid_read, invalid_write},
  {"stdout", 0, 0, 0,invalid_read, invalid_write},
  {"stderr", 0, 0, 0,invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

int fs_open(const char* pathname){
	int i;
	for(i = 0;i < NR_FILES;i++){
		if(strcmp(pathname,file_table[i].name) == 0){
			printf("fs_open returns %d\n",i);
			return i;
		}	
	}
	panic("file not found.\n");
	return -1;
}

size_t fs_filesz(int fd){
	printf("fs_filesz returns %d\n",file_table[fd].size);
	return file_table[fd].size;
}

size_t fs_read(int fd, void* buf, size_t len){
	assert(file_table[fd].open_offset+len <= file_table[fd].size);
	printf("read address:%d\n",file_table[fd].disk_offset+file_table[fd].open_offset);
	printf("read value: %s\n",(char*)buf);
	//printf("fs_read returns %d\n",ramdisk_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len));
	return ramdisk_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len);
}

size_t fs_write(int fd, const void *buf, size_t len){
	assert(file_table[fd].open_offset+len <= file_table[fd].size);
	printf("fs_write returns %d\n",ramdisk_write(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len));
	return ramdisk_write(buf,file_table[fd].disk_offset,len);
}

size_t fs_lseek(int fd, size_t offset, int whence){
	switch(whence){
		case SEEK_SET:
			file_table[fd].open_offset = offset;
			break;
		case SEEK_CUR:
			file_table[fd].open_offset += offset;
			break;
		case SEEK_END:
			file_table[fd].open_offset = file_table[fd].size;
	}
	assert(file_table[fd].open_offset <= file_table[fd].size);
	printf("fs_lseek returns %d\n",file_table[fd].open_offset);
	return file_table[fd].open_offset;
	
}

int fs_close(int fd){
	printf("fs_close returns 0\n");
	return 0;
}

