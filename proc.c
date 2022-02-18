#include "proc.h"

#include "fs.h"

void exec(struct file *f) {
  void *start_addr = (void *) f->data;
  // TODO: need lea?
  asm volatile("call *%[start_addr]" ::[start_addr] "r"(start_addr));
}
