#include "acpi.h"
#include "print.h"

struct XSDT *xsdt;

void init_acpi(void *rsdp) {
  xsdt = (struct XSDT *)((struct RSDP *) rsdp)->XsdtAddress;
}

void dump_sdth_signature(struct SDTH *sdth){
	unsigned char i;
	for (i = 0; i < 4; i++){
		putc(sdth->Signature[i]);
	}
}

void dump_xsdt_entries(){
	// Dump XSDT's SDTH
	dump_sdth_signature(&(xsdt->Header));
	puts(" ");

	// Number of entries
	unsigned long long num_entries = (xsdt->Header.Length - sizeof(struct SDTH)) / sizeof(struct SDTH *);

	unsigned long long i;
	for (i = 0; i < num_entries; i++){
		dump_sdth_signature(xsdt->Entry[i]);
		puts(" ");
	}
}
