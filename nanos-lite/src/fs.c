#include <fs.h>
//#include <assert.h>
size_t ramdisk_read(void* , size_t , size_t );
size_t ramdisk_write(const void* , size_t , size_t );
typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

size_t serial_write(const void *, size_t , size_t );
size_t events_read(void *, size_t , size_t );
size_t dispinfo_read(void *, size_t , size_t );

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB,FD_EVENT,FD_DISPINFO};

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
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, serial_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
  [FD_EVENT]  = {"/dev/events", 0, 0, events_read, invalid_write},
  [FD_DISPINFO]   = {"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write},
  [FD_FB] = {"/dev/fb", 0, 0, invalid_read, invalid_write},
#include "files.h"
};
static int num=sizeof(file_table)/sizeof(Finfo);
int fs_open(const char *pathname ,int flags ,int mode){
	for(int i=0;i<num;i++){
	if(strcmp(pathname,file_table[i].name)==0){
		file_table[i].open_offset=0;
		return i;
	}
	}
	printf("can't open\n");
	//assert(0);
	return -1;	
}
size_t fs_read(int fd,void *buf, size_t len){
	size_t t=0;
	//printf("%d\n",fd);
	//printf("into fs_read\n");
	if(file_table[fd].read!=NULL){t=file_table[fd].read(buf,0,len);}
	//printf("shouldnot reach here\n");
	size_t offset=file_table[fd].disk_offset+file_table[fd].open_offset;
	if(t) return t;
	if(len+file_table[fd].open_offset<=file_table[fd].size){
	t=ramdisk_read(buf,offset,len);
	}
	else{
	t=ramdisk_read(buf,offset,file_table[fd].size-file_table[fd].open_offset);
	}
	file_table[fd].open_offset+=t;
	return t;
}
size_t fs_write(int fd,const void *buf, size_t len){
	size_t t=0;
	if(file_table[fd].write!=NULL) t=file_table[fd].write(buf,0,len);
	size_t offset=file_table[fd].disk_offset+file_table[fd].open_offset;
	if(t) return t;
	if(len+file_table[fd].open_offset<=file_table[fd].size){
	t=ramdisk_write(buf,offset,len);
	}
	else{
	t=ramdisk_write(buf,offset,file_table[fd].size-file_table[fd].open_offset);
	}
	file_table[fd].open_offset+=t;
	return t;
}
size_t fs_lseek(int fd,size_t offset, int whence){
	switch(whence){
	case SEEK_SET:
	file_table[fd].open_offset=offset;break;
	case SEEK_CUR:
	file_table[fd].open_offset+=offset;break;
	case SEEK_END:
	file_table[fd].open_offset=file_table[fd].size+offset;break;
	}
	return file_table[fd].open_offset;
}
int fs_close(int fd){
	file_table[fd].open_offset=0;
	return 0;
}
void init_fs() {
  // TODO: initialize the size of /dev/fb
}
