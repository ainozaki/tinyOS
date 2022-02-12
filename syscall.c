#include "syscall.h"

#include "intr.h"
#include "pic.h"
#include "print.h"

#define SYSCALL_INTR_NO 0x80

void syscall_handler();

void init_syscall() {
  // register IDT
  // (Kernel crashes without lea)
  void *handler;
  __asm__ volatile("lea syscall_handler, %[handler]" : [handler] "=r"(handler));
  set_intr(SYSCALL_INTR_NO, handler);

  // Deactive PIC mask
  enable_pic_intr(SYSCALL_INTR_NO);
}

unsigned long long do_syscall_interrupt(unsigned long long syscall_id,
                                        unsigned long long arg1,
                                        unsigned long long arg2,
                                        unsigned long long arg3) {
  unsigned long long ret = 0xcafe;

  puts("SYSCALL_ID:");
  puth(syscall_id, 16);
  puts("\r\n");
  puts("ARGS:");
  puth(arg1, 16);
  puts(" ");
  puth(arg2, 16);
  puts(" ");
  puth(arg3, 16);
  puts("\r\n");

  // End of Interrupt
  set_pic_eoi(SYSCALL_INTR_NO);

  return ret;
}
