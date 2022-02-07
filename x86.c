#include "x86.h"

unsigned char io_read(unsigned short address) {
    unsigned char value;
    __asm__ volatile("inb %[address], %[value]"
                     : [value] "=a"(value)
                     : [address] "d"(address));
    return value;
}
