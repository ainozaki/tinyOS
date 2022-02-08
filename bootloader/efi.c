#include "efi.h"

#include "common.h"

struct EFI_SYSTEM_TABLE *ST;
struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;
struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;

// Local function cannot define this size of variable.
unsigned char mem_desc[4800];

void efi_init(struct EFI_SYSTEM_TABLE *SystemTable) {
  ST = SystemTable;
  ST->ConOut->ClearScreen(ST->ConOut);

  // Most of the protocols are available through SystemTable->BootServices
  struct EFI_GUID gop_guid = {0x9042a9de,
                              0x23dc,
                              0x4a38,
                              {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};
  struct EFI_GUID sfsp_guid = {
          0x0964e5b22,
          0x6459,
          0x11d2,
          {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
  // Watch Dog Timer. Default timer is 5 min.
  ST->BootServices->SetWatchdogTimer(/*Timeout=*/0, /*WatchDogCode=*/0,
                                     /*DataSize=*/0, /*WatchDogData*/ NULL);
  ST->BootServices->LocateProtocol(&sfsp_guid, NULL, (void **) &SFSP);
  ST->BootServices->LocateProtocol(&gop_guid, NULL, (void **) &GOP);
}

void exit_boot_services(void *ImageHandle) {
  unsigned long long status;
  unsigned long long mmap_size;
  unsigned long long map_key;
  unsigned long long mem_desc_unit_size;
  unsigned int desc_ver;

  do {
    put(L"GetMemoryMap\r\n");
    mmap_size = 4800;
    status = ST->BootServices->GetMemoryMap(
            &mmap_size, (struct EFI_MEMORY_DESCRIPTOR *) mem_desc, &map_key,
            &mem_desc_unit_size, &desc_ver);
  } while (status);

  status = ST->BootServices->ExitBootServices(ImageHandle, map_key);
  assert(status, L"ExitBootServices");
}

void dump_efi_conf_table() {
  unsigned long long i;
  for (i = 0; i < ST->NumberOfTableEntries; i++) {
    puth(i, 1);
    putc(L':');
    puth((unsigned long long) &ST->ConfigurationTable[i], 16);
    putc(L':');
    puth(ST->ConfigurationTable[i].VendorGuid.Data1, 8);
    putc(L' ');
    puth(ST->ConfigurationTable[i].VendorGuid.Data2, 4);
    putc(L' ');
    puth(ST->ConfigurationTable[i].VendorGuid.Data3, 4);
    putc(L' ');
    unsigned char j;
    for (j = 0; j < 8; j++)
      puth(ST->ConfigurationTable[i].VendorGuid.Data4[j], 2);
    putc(L':');
    puth((unsigned long long) ST->ConfigurationTable[i].VendorTable, 16);
    put(L"\r\n");
  }
}

void *find_efi_acpi_table(void) {
  const struct EFI_GUID acpi_guid = {
          0x8868e871,
          0xe4f1,
          0x11d3,
          {0xbc, 0x22, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81}};
  unsigned long long i;
  unsigned char is_equal = TRUE;

  for (i = 0; i < ST->NumberOfTableEntries; i++) {
    struct EFI_GUID *guid = &(ST->ConfigurationTable[i].VendorGuid);
    if (guid->Data1 == acpi_guid.Data1) {
      if (guid->Data2 == acpi_guid.Data2) {
        if (guid->Data3 == acpi_guid.Data3) {
          unsigned char j;
          for (j = 0; j < 8; j++) {
            if (guid->Data4[j] != acpi_guid.Data4[j]) {
              is_equal = FALSE;
            }
          }
          if (is_equal == TRUE) {
            return ST->ConfigurationTable[i].VendorTable;
          }
        }
      }
    }
  }
  return NULL;
}
