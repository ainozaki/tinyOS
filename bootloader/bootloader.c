#include "common.h"
#include "efi.h"
#include "file.h"

#define KERNEL_NAME L"kernel.bin"
#define KERNEL_START 0x0000000000110000

struct fb {
  unsigned long long base;
  unsigned long long size;
  unsigned int hr;
  unsigned int vr;
};

struct platform_info {
  struct fb fb;
  void *rsdp;
};

void efi_main(void *ImageHandle __attribute__((unused)),
              struct EFI_SYSTEM_TABLE *SystemTable) {
  unsigned long long status;

  // Initialize
  efi_init(SystemTable);

  put(L"Hello BootLoader!!\r\n");

  // Open kernel file
  put(L"Open kernel file ... ");
  struct EFI_FILE_PROTOCOL *root;
  struct EFI_FILE_PROTOCOL *kernel_file;
  status = SFSP->OpenVolume(SFSP, &root);
  assert(status, L"SFSP->OpenVolume");

  status = root->Open(root, &kernel_file, KERNEL_NAME, EFI_FILE_MODE_READ,
                      /*Attributes=*/0);
  assert(status, L"root->Open(kernel)");
  put(L"done\r\n");

  // Load kernel file
  unsigned long long kernel_start = KERNEL_START;
  unsigned long long stack_base = kernel_start + (1 * 1024 * 1024);// 1MB
  unsigned long long kernel_size = sizeof_file(kernel_file);

  struct header {
    unsigned char _buf[0x120];
    void *bss_start;
    unsigned long long bss_size;
  } head;
  put(L"Load kernel head ...");
  file_read(kernel_file, (void *) &head, sizeof(head));
  put(L"done\r\n");

  kernel_size -= sizeof(head);
  put(L"Load kernel body ...");
  file_read(kernel_file, (void *) kernel_start, kernel_size);
  put(L"done\r\n");

  put(L"Close kernel_file...");
  kernel_file->Close(kernel_file);
  put(L"done\r\n");

  // Debug
  put(L"kernel body first 16 bytes: 0x");
  put_n_bytes((unsigned char *) kernel_start, 16);
  put(L"\r\n");
  put(L"kernel body last 16 bytes: 0x");
  unsigned char *kernel_last =
          (unsigned char *) (kernel_start + kernel_size - 16);
  put_n_bytes(kernel_last, 16);
  put(L"\r\n");

  // Zero clear bss section
  put(L"Zero clear bss section... ");
  ST->BootServices->SetMem(head.bss_start, head.bss_size, 0);
  put(L"done\r\n");

  // Dump configuration table
  put(L"Dump configuration table...\r\n");
  put(L"index:struct address:GUIT.Data1 Data2 Data3 Data4:Table address\r\n");
  dump_efi_conf_table();
  put(L"done\r\n");

  // Get ACPI table
  // Top of ACPI table is ASCII signature.
  put(L"Get ACPI table... ");
  char *acpi_table = find_efi_acpi_table();
  put(L"done\r\n");
  putc(*acpi_table++);
  putc(*acpi_table++);
  putc(*acpi_table++);
  putc(*acpi_table++);
  putc(*acpi_table++);
  putc(*acpi_table++);
  putc(*acpi_table++);
  putc(*acpi_table++);
  putc(*acpi_table);

  // Pass to kernel
  unsigned long long kernel_arg1 = (unsigned long long) ST;

  struct fb fb;
  fb.base = GOP->Mode->FrameBufferBase;
  fb.size = GOP->Mode->FrameBufferSize;
  fb.hr = GOP->Mode->Info->HorizontalResolution;
  fb.vr = GOP->Mode->Info->VerticalResolution;

  struct platform_info platform_info;
  platform_info.fb.base = fb.base;
  platform_info.fb.size = fb.size;
  platform_info.fb.hr = fb.hr;
  platform_info.fb.vr = fb.vr;
  platform_info.rsdp = find_efi_acpi_table();

  unsigned long long kernel_arg2 = (unsigned long long) &platform_info;

  unsigned long long kernel_arg3 = 0;// App start. Future work.

  put_param(L"kernel_arg1", kernel_arg1);
  put_param(L"kernel_arg2", kernel_arg2);
  put_param(L"kernel_arg3", kernel_arg3);
  put_param(L"stack_base", stack_base);
  put_param(L"kernel_start", kernel_start);

  put(L"ExitBootServices... ");
  exit_boot_services(ImageHandle);


  // Call kernel
  __asm__("	mov	%0, %%rdx\n"
          "	mov	%1, %%rsi\n"
          "	mov	%2, %%rdi\n"
          "	mov	%3, %%rsp\n"
          "	jmp	*%4\n" ::"m"(kernel_arg3),
          "m"(kernel_arg2), "m"(kernel_arg1), "m"(stack_base),
          "m"(kernel_start));

  while (TRUE)
    ;
}
