#ifndef INTR_H_
#define INTR_H_

void set_intr(unsigned char intr_no, void *handler);

void intr_init();

#endif// INTR_H_
