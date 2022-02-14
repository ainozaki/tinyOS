#include "syscall.h"

#include "intr.h"
#include "pic.h"
#include "print.h"

#define SYSCALL_INTR_NO 0x80

enum SYSCALL_NO {
  SYSCALL_PUTC,
  SYSCALL_MAX,
};

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
                                        unsigned long long arg2
                                        __attribute__((unused)),
                                        unsigned long long arg3
                                        __attribute__((unused))) {
  unsigned long long ret = 0;

  switch (syscall_id) {
    case SYSCALL_PUTC:
      putc((char) arg1);
      break;
  }

  // End of Interrupt
  set_pic_eoi(SYSCALL_INTR_NO);

  return ret;
}
