#include "intr.h"

#include "print.h"
#include "x86.h"

#define DESC_TYPE_INTR 14

// Interrupt Descriptor Table
struct interrupt_descriptor idt[MAX_INTR_NO];

// For IDTR
unsigned long long idtr[2];

void default_handler();

void set_intr(unsigned char intr_no, void *handler) {
  if (!handler) {
    puts("NULL HANDLER\r\n");
    __asm__ volatile("hlt");
  }
  /*
  puts("INTR HANDLER: NO:");
  putd(intr_no, 2);
  puts(" HANDLER_ADDR:");
  puth(handler, 16);
  puts("\r\n");
	*/
  idt[intr_no].offset_00_15 = (unsigned long long) handler;
  idt[intr_no].segment_selector = SS_KERNEL_CODE;
  idt[intr_no].type = DESC_TYPE_INTR;
  idt[intr_no].p = 1;
  idt[intr_no].offset_31_16 = (unsigned long long) handler >> 16;
  idt[intr_no].offset_63_32 = (unsigned long long) handler >> 32;
}

void intr_init() {
  // Set default handler
  int i;
  void *handler;
  __asm__ volatile("lea default_handler, %[handler]" : [handler] "=r"(handler));
  for (i = 0; i < MAX_INTR_NO; i++) {
    set_intr(i, handler);
  }

  // Set IDTR
  idtr[0] = ((unsigned long long) idt << 16) | (sizeof(idt) - 1);
  idtr[1] = ((unsigned long long) idt >> 48);
  __asm__ volatile("lidt idtr");
}
