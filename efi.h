#ifndef EFI_H_
#define EFI_H_

struct EFI_INPUT_KEY {
  unsigned short ScanCode;
  unsigned short UnicodeChar;
};

struct EFI_SYSTEM_TABLE {
  char _buf1[44];

  struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    unsigned long long _buf;

    unsigned long long (*ReadKeyStroke)(
        struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This, struct EFI_INPUT_KEY *Key);

    void *WaitForKey;
  } * ConIn;

  unsigned long long _buf2;

  struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    unsigned long long _buf;

    unsigned long long (*OutputString)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, unsigned short *String);

    unsigned long long _buf2[4];

    unsigned long long (*ClearScreen)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
  } * ConOut;
  unsigned long long _buf[3];
  struct EFI_BOOT_SERVICES {
    char _buf1[24];
    unsigned long long _buf2[9];
    unsigned long long (*WaitForEvent)(unsigned long long NumberOfEvents,
                                       void **Event, unsigned long long *Index);
  } * BootServices;
};

void efi_init(struct EFI_SYSTEM_TABLE *SystemTable);

void put(unsigned short *str);

void wait_and_get_line(unsigned short *str, unsigned int max);

#endif // EFI_H_
