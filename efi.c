#include "efi.h"

#include "common.h"

struct EFI_SYSTEM_TABLE *ST;
struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;

void efi_init(struct EFI_SYSTEM_TABLE *SystemTable) {
  ST = SystemTable;

  ST->BootServices->SetWatchdogTimer(0, 0, 0, NULL);

  // GUID of EFI_GRAPHICS_OUTPUT_PROTOCOL
  // Get Protocol's top address using SystemTable->BootServices
  struct EFI_GUID gop_guid = {0x9042a9de,
                              0x23dc,
                              0x4a38,
                              {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};
  ST->BootServices->LocateProtocol(&gop_guid, NULL, (void **)&GOP);
}

void putc(unsigned short c) {
  unsigned short str[2] = L"";
  str[0] = c;
  ST->ConOut->OutputString(ST->ConOut, str);
}

void put(unsigned short *str) { ST->ConOut->OutputString(ST->ConOut, str); }

unsigned short wait_and_get() {
  struct EFI_INPUT_KEY key;
  unsigned long long waitidx;
  while (1) {
    ST->BootServices->WaitForEvent(1, &(ST->ConIn->WaitForKey), &waitidx);
    ST->ConIn->ReadKeyStroke(ST->ConIn, &key);
    return key.UnicodeChar;
  }
}

void wait_and_get_line(unsigned short *str, unsigned int max) {
  unsigned int i = 0;
  while (i < max) {
    str[i] = wait_and_get();
    putc(str[i]);
    if (str[i] == L'\r') {
      putc('\n');
      break;
    }
    i++;
  }
  str[i] = '\0';
}
