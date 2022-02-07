#include "x86.h"

#define SS_KERNEL_CODE 0x0008

const unsigned long long gdt[] = {
	0x0000000000000000, // NULL descriptor
	0x00af9a000000ffff, // Code Segment(r-x), base=0, limit=4GB
	0x00cf93000000ffff  // Data Segment(rw-), base=0, limit=4GB 
};

unsigned long long gdtr[2];

unsigned char io_read(unsigned short address) {
    unsigned char value;
    __asm__ volatile("inb %[address], %[value]"
                     : [value] "=a"(value)
                     : [address] "d"(address));
    return value;
}

void gdt_init(){
	// Set GDTR
	gdtr[0] = ((unsigned long long) gdt << 16) | (sizeof(gdt) - 1);
	gdtr[1] = (unsigned long long) gdt >> 48;
	__asm__ volatile ("lgdt gdtr");

	// Set DS(Data Segment) and SS(Stack Segment)
	__asm__ volatile ("movw $2*8, %ax\n"
									 "movw %ax, %ds\n"
									 "movw %ax, %ss\n");

	// Set CS(Code Stack)
	// Switch current CS
	unsigned short selector = SS_KERNEL_CODE;
	unsigned long long dummy;
	__asm__ volatile ("pushq %[selector];"
										"leaq ret_label(%%rip), %[dummy];"
										"pushq %[dummy];"
										"lretq;"
										"ret_label:"
										: [dummy]"=r"(dummy)
										: [selector]"m"(selector));
}
