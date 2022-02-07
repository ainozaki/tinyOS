#ifndef PIC_H_
#define PIC_H_

void pic_init();

// Only Master PIC
void enable_pic_intr(unsigned char intr_no);

void set_pic_eoi(unsigned char intr_no);

#endif// PIC_H_
