#ifndef FS_H_
#define FS_H_

#define FILE_NAME_LEN 28

struct file {
  char filename[FILE_NAME_LEN];
  unsigned int size;
  unsigned char data[0];
};

void fs_init(void *fs_start);
struct file *open(char *filename);

#endif// FS_H_
