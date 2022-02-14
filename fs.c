#include "fs.h"

#include "common.h"
#include "print.h"

#define EOF 0x00

struct file *file_start;

void fs_init(void *fs_start) {
  file_start = fs_start;
}

struct file *open(char *filename) {
  struct file *f = file_start;
  while (f->filename[0] != EOF) {
    if (!strcmp(filename, f->filename)) {
      return f;
    }
    f = (struct file *) ((unsigned long long) f + sizeof(struct file) +
                         f->size);
  }
  return NULL;
}
