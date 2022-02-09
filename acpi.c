#include "acpi.h"
#include "common.h"
#include "print.h"

struct XSDT *xsdt;
unsigned long long num_entries;

void init_acpi(void *rsdp) {
	// Get XDST from RSDP
  xsdt = (struct XSDT *)((struct RSDP *) rsdp)->XsdtAddress;
	
	// Number of entries
	num_entries = (xsdt->Header.Length - sizeof(struct SDTH)) / sizeof(struct SDTH *);

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

	unsigned long long i;
	for (i = 0; i < num_entries; i++){
		dump_sdth_signature(xsdt->Entry[i]);
		puts(" ");
	}
}

struct SDTH *get_sdth(char *signature){
	unsigned long long i;
	for (i = 0; i < num_entries; i++){
		if (strncmp(xsdt->Entry[i]->Signature, signature, 4) == 0){
			return xsdt->Entry[i];
		}
	}
	return NULL;
}
