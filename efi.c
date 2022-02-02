#include "efi.h"

struct EFI_SYSTEM_TABLE *ST;

void efi_init(struct EFI_SYSTEM_TABLE *SystemTable) { ST = SystemTable; }

void putc(unsigned short c) {
	unsigned short str[2] = L"";
	str[0] = c;
	ST->ConOut->OutputString(ST->ConOut, str); 
}

void put(unsigned short *str){
	ST->ConOut->OutputString(ST->ConOut, str); 
}

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
