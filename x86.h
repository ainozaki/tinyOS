#ifndef X86_H_
#define X86_H_

// read |address| through i/o instruction.
unsigned char io_read(unsigned short address);

// Initialize Grobal Desctiprot Table.
void gdt_init();

#endif// X86_H_
