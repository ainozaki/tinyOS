#include "pic.h"

#include "print.h"
#include "x86.h"

#define MPIC_ICW1_ADDR 0x0020
#define MPIC_OCW2_ADDR 0x0020
#define MPIC_ICW2_ADDR 0x0021
#define MPIC_ICW3_ADDR 0x0021
#define MPIC_ICW4_ADDR 0x0021
#define MPIC_OCW1_ADDR 0x0021
#define MPIC_IMR_ADDR 0x0021

#define SPIC_ICW1_ADDR 0x00a0
#define SPIC_ICW2_ADDR 0x00a1
#define SPIC_ICW3_ADDR 0x00a1
#define SPIC_ICW4_ADDR 0x00a1
#define SPIC_OCW1_ADDR 0x00a1

#define PIC_OCW2_BIT_MANUAL_EOI 0x60

#define INTR_NO_BASE_MASTER 32
#define INTR_NO_BASE_SLAVE 40

void pic_init(void) {
  // Master PIC
  io_write(MPIC_ICW1_ADDR, 0x11);
  io_write(MPIC_ICW2_ADDR, INTR_NO_BASE_MASTER);
  io_write(MPIC_ICW3_ADDR, 0x04);
  io_write(MPIC_ICW4_ADDR, 0x01);
  io_write(MPIC_OCW1_ADDR, 0xff);

  // Slave PIC
  io_write(SPIC_ICW1_ADDR, 0x11);
  io_write(SPIC_ICW2_ADDR, INTR_NO_BASE_SLAVE);
  io_write(SPIC_ICW3_ADDR, 0x02);
  io_write(SPIC_ICW4_ADDR, 0x01);
  io_write(SPIC_OCW1_ADDR, 0xff);
}

void enable_pic_intr(unsigned char intr_no) {
  puts("ENABLE_PIC_INTR");
  unsigned char ir_no = intr_no - INTR_NO_BASE_MASTER;
  unsigned char ir_bit = 1U << ir_no;

  unsigned char mask = io_read(MPIC_IMR_ADDR);
  if (!(ir_bit & mask)) {
    return;
  }
  io_write(MPIC_OCW1_ADDR, mask - ir_bit);
}

void set_pic_eoi(unsigned char intr_no) {
  unsigned char ir_no = intr_no - INTR_NO_BASE_MASTER;
  io_write(MPIC_OCW2_ADDR, PIC_OCW2_BIT_MANUAL_EOI | ir_no);
}
