#include "efi.h"

#include "common.h"

struct EFI_SYSTEM_TABLE *ST;
struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
struct EFI_SIMPLE_POINTER_PROTOCOL *SPP;
void efi_init(struct EFI_SYSTEM_TABLE *SystemTable) {
    ST = SystemTable;

    // GUID of EFI_GRAPHICS_OUTPUT_PROTOCOL
    // Set GOP(Protocol's top address) using SystemTable->BootServices
    // Most of the protocols are available through SystemTable->BootServices
    struct EFI_GUID gop_guid = {0x9042a9de,
                                0x23dc,
                                0x4a38,
                                {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};

    // GUID of EFI_SIMPLE_POINTER_PROTOCOL
    struct EFI_GUID spp_guid = {0x31878c87, 0xb75, 0x11d5, {0x9a, 0x4f, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d}};

    // Watch Dog Timer. Default timer is 5 min.
    ST->BootServices->SetWatchdogTimer(/*Timeout=*/0, /*WatchDogCode=*/0, /*DataSize=*/0, /*WatchDogData*/NULL);

    ST->BootServices->LocateProtocol(&gop_guid, NULL, (void **) &GOP);
    ST->BootServices->LocateProtocol(&spp_guid, NULL, (void **)SPP);
}

void putc(unsigned short c) {
    // One character is 2-byte in UEFI.
    unsigned short str[2] = L"";
    str[0] = c;
    ST->ConOut->OutputString(ST->ConOut, str);
}

void put(unsigned short *str) { ST->ConOut->OutputString(ST->ConOut, str); }

void puth(unsigned long long val, unsigned char len){
    int i;
    unsigned short unicode;
    unsigned short str[100];

    for (i = len - 1; i >= 0; i--){
        unicode = (unsigned short)(val & 0x0f);
        if (unicode < 0xa){
            str[i] = L'0' + unicode;
        }else {
            str[i] = L'A' + (unicode - 0xa);
        }
        val >>= 4;
    }
    str[len] = L'\0';
    put(str);
}

unsigned short wait_and_get() {
    struct EFI_INPUT_KEY key;
    unsigned long long waitidx;
    while (1) {
        ST->BootServices->WaitForEvent(/*NumberOfEvents=*/1, &(ST->ConIn->WaitForKey), &waitidx);
        // ReadKeyStroke is non-blocking.
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
