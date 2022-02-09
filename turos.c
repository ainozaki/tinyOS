#include "acpi.h"
#include "intr.h"
#include "kbc.h"
#include "pic.h"
#include "pixel.h"
#include "print.h"
#include "x86.h"

struct platform_info {
  struct framebuffer fb;
  void *rsdp;
};

// @_t: SystemTable address
void start_kernel(void *_t __attribute__((unused)),
                  struct platform_info *platform_info,
                  void *_fs_start __attribute__((unused))) {
  fb_init(&platform_info->fb);

  // paint screen
  set_screen(64, 224, 208);

  // display character
  puts("HELLO TUROS\r\n");

  // Initialize CPU
  puts("INTIALIZE GDT AND INTR... ");
  gdt_init();
  intr_init();
  puts("DONE\r\n");

  // Initialize IC
  puts("INITIALIZE PIC AND KBC...");
  pic_init();
  kbc_intr_init();
  puts("DONE\r\n");

  // Enable interrupt CPU
  puts("ENABLE CPU INTR...");
  enable_cpu_intr();
  puts("DONE\r\n");

  // Dump RSDP signature
  puts("DUMP RSDP SIGNATURE: ");
  char *rsdp = (char *) platform_info->rsdp;
  putc(*rsdp++);
  putc(*rsdp++);
  putc(*rsdp++);
  putc(*rsdp++);
  putc(*rsdp++);
  putc(*rsdp++);
  putc(*rsdp++);
  putc(*rsdp);
  puts(" ...DONE\r\n");

  // Register XSDT
  puts("REGISTER XSDT...");
  init_acpi(platform_info->rsdp);
  puts("DONE\r\n");

	// Dump XSDT entries
	puts("DUMP XSDT ENTRIES: ");
	dump_xsdt_entries();
	puts("...DONE\r\n");

  while (1) {
    __asm__ volatile("hlt");
  }
}
