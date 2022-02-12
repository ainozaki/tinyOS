#include "hpet.h"

#include "acpi.h"
#include "common.h"
#include "intr.h"
#include "pic.h"
#include "print.h"

#define TIMER_N 0
#define US_TO_FS 1000000000

unsigned long long hpet_reg;
unsigned int counter_clk_period;

//void (*user_handler)(unsigned long long rsp) = NULL;

// GCIDR (General Capabilities and ID Register)
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

// GCR (General Configuration Register)
#define GCR_ADDR (hpet_reg + 0x10)
#define GCR (*(volatile unsigned long long *) GCR_ADDR)
union gcr {
  unsigned long long raw;
  struct __attribute__((packed)) {
    unsigned long long enable_cnf : 1;
    unsigned long long leg_rt_cnf : 1;
    unsigned long long _reserved : 62;
  };
};

// MCR (Main Counter Register)
#define MCR_ADDR (hpet_reg + 0xf0)
#define MCR (*(volatile unsigned long long *) MCR_ADDR)

// TNCCR (Timer N Configuration and Capabilities Register)
#define TNCCR_ADDR(n) (hpet_reg + (0x20 * (n)) + 0x100)
#define TNCCR(n) (*(volatile unsigned long long *) (TNCCR_ADDR(n)))
union tnccr {
  unsigned long long raw;
  struct __attribute__((packed)) {
    unsigned long long _reserved1 : 1;
    unsigned long long int_type_cnf : 1;
    unsigned long long int_enb_cnf : 1;
    unsigned long long type_cnf : 1;
    unsigned long long per_int_cap : 1;
    unsigned long long size_cap : 1;
    unsigned long long val_set_cnf : 1;
    unsigned long long _reserved2 : 1;
    unsigned long long mode32_cnf : 1;
    unsigned long long int_route_cnf : 5;
    unsigned long long fsb_en_cnf : 1;
    unsigned long long fsb_int_del_cap : 1;
    unsigned long long _reserved3 : 16;
    unsigned long long int_route_cap : 32;
  };
};

// TNCR (TImer N COmparator Register)
#define TNCR_ADDR(n) (hpet_reg + (0x20 * (n)) + 0x108)
#define TNCR(n) (*(volatile unsigned long long *) (TNCR_ADDR(n)))

void hpet_handler(void);

void init_hpet() {
  struct HPET_TABLE *hpet_table = (struct HPET_TABLE *) get_sdth("HPET");
  check_nullptr((void *) hpet_table, "HPET_TABLE");

  hpet_reg = hpet_table->base_address.address;

  // Disable HPET
  union gcr gcr;
  gcr.raw = GCR;
  gcr.enable_cnf = 0;
  gcr.leg_rt_cnf = 1;
  GCR = gcr.raw;

  // Get counter period
  union gcidr gcidr;
  gcidr.raw = GCIDR;
  counter_clk_period = gcidr.counter_clk_period;

  // Interrupt setting
  union tnccr tnccr;
  tnccr.raw = TNCCR(TIMER_N);
  tnccr.int_type_cnf = 0;// EDGE
  tnccr.int_enb_cnf = 0;
  tnccr.type_cnf = 0;// NON_PREIODIC
  tnccr.val_set_cnf = 0;
  tnccr.mode32_cnf = 0;
  tnccr.fsb_en_cnf = 0;
  tnccr._reserved1 = 0;
  tnccr._reserved2 = 0;
  tnccr._reserved3 = 0;
  TNCCR(TIMER_N) = tnccr.raw;

  // register IDT
  // (Kernel crashes without lea)
  void *handler;
  __asm__ volatile("lea hpet_handler, %[handler]" : [handler] "=r"(handler));
  set_intr(HPET_INTR_NO, handler);

  // Deactive PIC mask
  enable_pic_intr(HPET_INTR_NO);
}

void dump_gcidr() {
  puts("[GCIDR]\r\n");
  union gcidr gcidr;
  gcidr.raw = GCIDR;

  puts("REV_ID: ");
  putd(gcidr.rev_id, 3);
  puts("\r\n");

  puts("NUM_TIM_CAP: ");
  putd(gcidr.num_tim_cap, 5);
  puts("\r\n");

  puts("COUNT_SIZE_CAP: ");
  putd(gcidr.count_size_cap, 1);
  puts("\r\n");

  puts("LEG_RT_CAP: ");
  putd(gcidr.leg_rt_cap, 1);
  puts("\r\n");

  puts("VENDOR_ID: ");
  putd(gcidr.vendor_id, 16);
  puts("\r\n");

  puts("COUNTER_CLK_PERIOD: ");
  putd(gcidr.counter_clk_period, 16);
  puts("\r\n");
}

void dump_gcr() {
  puts("[GCR]\r\n");
  union gcr gcr;
  gcr.raw = GCR;

  puts("ENABLE_CNF: ");
  putd(gcr.enable_cnf, 1);
  puts("\r\n");

  puts("LEG_RT_CNF: ");
  putd(gcr.leg_rt_cnf, 1);
  puts("\r\n");
}

void dump_mcr() {
  puts("[MCR] ");
  putd(MCR, 16);
  puts("\r\n");
}

void sleep(unsigned long long us) {
  // Caluculate main counter of us seconds after.
  unsigned long long main_counter_now = MCR;
  unsigned long long fs = us * 1000000000;
  union gcidr gcidr;
  gcidr.raw = GCIDR;
  unsigned long long main_counter_duration = fs / gcidr.counter_clk_period;
  unsigned long long main_counter_after =
          main_counter_now + main_counter_duration;

  // Enable HPET
  union gcr gcr;
  gcr.raw = GCR;
  unsigned char to_disable = 0;
  if (!gcr.enable_cnf) {
    puts("ENABLE HPET ");
    gcr.enable_cnf = 1;
    GCR = gcr.raw;
    to_disable = 1;
  }

  while (MCR < main_counter_after) {
  }

  if (to_disable) {
    gcr.raw = GCR;
    gcr.enable_cnf = 0;
    GCR = gcr.raw;
  }
}


void alert(unsigned long long us) {
  // Enable non-preiodic interrupt
  union tnccr tnccr;
  tnccr.raw = TNCCR(TIMER_N);
  tnccr.int_enb_cnf = 1;
  tnccr.type_cnf = 0;// NON_PREIODIC
  tnccr._reserved1 = 0;
  tnccr._reserved2 = 0;
  tnccr._reserved3 = 0;
  TNCCR(TIMER_N) = tnccr.raw;

  // zero clear main-counter
  MCR = (unsigned long long) 0;

  // Comparator
  unsigned long long femt_sec = us * US_TO_FS;
  unsigned long long clk_counts = femt_sec / counter_clk_period;
  TNCR(TIMER_N) = clk_counts;

  // Enable HPET
  union gcr gcr;
  gcr.raw = GCR;
  gcr.enable_cnf = 1;
  GCR = gcr.raw;
}

void do_hpet_interrupt() {
  puts("DO_HPET_INTERRUPT\r\n");
  // disable HPET
  union gcr gcr;
  gcr.raw = GCR;
  gcr.enable_cnf = 0;
  GCR = gcr.raw;

  // disable interrupt
  union tnccr tnccr;
  tnccr.raw = TNCCR(TIMER_N);
  tnccr.int_enb_cnf = 0;
  tnccr._reserved1 = 0;
  tnccr._reserved2 = 0;
  tnccr._reserved3 = 0;
  TNCCR(TIMER_N) = tnccr.raw;

  // End Of Interrupt
  set_pic_eoi(HPET_INTR_NO);
}
