#ifndef INTR_H_
#define INTR_H_

// Set interrupt handler
void set_intr(unsigned char intr_no, void *handler);

// Initialize with default handler and set IDTR
void intr_init();

#endif// INTR_H_
