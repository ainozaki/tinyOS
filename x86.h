#ifndef X86_H_
#define X86_H_

#define MAX_INTR_NO 256
#define SS_KERNEL_CODE 0x0008
#define SS_KERNEL_DATA 0x0010

struct interrupt_descriptor {
  unsigned short offset_00_15;
  unsigned short segment_selector;
  unsigned short ist : 3;
  unsigned short _zero1 : 5;
  unsigned short type : 4;
  unsigned short _zero2 : 1;
  unsigned short dpl : 2;
  unsigned short p : 1;
  unsigned short offset_31_16;
  unsigned int offset_63_32;
  unsigned int _reserved;
};

// read |address| through i/o instruction.
unsigned char io_read(unsigned short address);

// Initialize Grobal Desctiprot Table.
void gdt_init();

// Enable CPU interrupt
void enable_cpu_intr();

#endif// X86_H_
