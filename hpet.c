#include "hpet.h"

#include "acpi.h"
#include "common.h"
#include "print.h"

unsigned long long hpet_reg;

// General Capabilities and ID Register
#define GCIDR_ADDR (hpet_reg)
#define GCIDR (*(volatile unsigned long long *) GCIDR_ADDR)
union gcidr {
  unsigned long long raw;
  struct __attribute__((packed)) {
    unsigned long long rev_id : 8;
    unsigned long long num_tim_cap : 5;
    unsigned long long count_size_cap : 1;
    unsigned long long _reserved : 1;
    unsigned long long leg_rt_cap : 1;
    unsigned long long vendor_id : 16;
    unsigned long long counter_clk_period : 32;
  };
};

// General Configuration Register
#define GCR_ADDR (hpet_reg + 0x10)
#define GCR (*(volatile unsigned long long *) GCR_ADDR)
union gcr {
	unsigned long long raw;
	struct __attribute__((packed)) {
		unsigned long long enable_cnf:1;
		unsigned long long leg_rt_cnf:1;
		unsigned long long _reserved:62;
	};
};


// Main Counter Register
#define MCR_ADDR (hpet_reg + 0xf0)
#define MCR (*(volatile unsigned long long*) MCR_ADDR)
void init_hpet() {
  struct HPET_TABLE *hpet_table = (struct HPET_TABLE *) get_sdth("HPET");
  check_nullptr((void *) hpet_table, "HPET_TABLE");

  hpet_reg = hpet_table->base_address.address;
}

void dump_gcidr(){
	puts("GCIDR\r\n");
	union gcidr gcidr;
	gcidr.raw = GCIDR;

	puts("REV ID ");
	putd(gcidr.rev_id, 3);
	puts("\r\n");

	puts("NUM TIM CAP ");
	putd(gcidr.num_tim_cap, 5);
	puts("\r\n");

	puts("COUNT SIZE CAP ");
	putd(gcidr.count_size_cap, 1);
	puts("\r\n");
	
	puts("LEG RT CAP ");
	putd(gcidr.leg_rt_cap, 1);
	puts("\r\n");

	puts("VENDOR ID ");
	putd(gcidr.vendor_id, 16);
	puts("\r\n");

	puts("COUNTER CLK PERIOD ");
	putd(gcidr.counter_clk_period, 16);
	puts("\r\n");
}

void dump_gcr(){
	puts("GCR\r\n");
	union gcr gcr;
	gcr.raw = GCR;

	puts("ENABLE CNF ");
	putd(gcr.enable_cnf, 1);
	puts("\r\n");

	puts("LEG RT CNF ");
	putd(gcr.leg_rt_cnf, 1);
	puts("\r\n");
}

void dump_mcr(){
	puts("MCR\r\n");
	putd(MCR, 16);
	puts("\r\n");
}
