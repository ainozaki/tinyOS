#include "efi.h"

#include "common.h"

struct EFI_SYSTEM_TABLE *ST;
struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
struct EFI_SIMPLE_POINTER_PROTOCOL *SPP;
struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;
struct EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *DPTTP;
struct EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL *DPFTP;

struct EFI_GUID lip_guid = {0x5b1b31a1,
                            0x9562,
                            0x11d2,
                            {0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

void efi_init(struct EFI_SYSTEM_TABLE *SystemTable) {
    ST = SystemTable;

    // GUID of EFI_GRAPHICS_OUTPUT_PROTOCOL
    // Set GOP(Protocol's top address) using SystemTable->BootServices
    // Most of the protocols are available through SystemTable->BootServices
    struct EFI_GUID gop_guid = {0x9042a9de,
                                0x23dc,
                                0x4a38,
                                {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};

    struct EFI_GUID spp_guid = {0x31878c87,
                                0xb75,
                                0x11d5,
                                {0x9a, 0x4f, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d}};
    struct EFI_GUID sfsp_guid = {
            0x0964e5b22,
            0x6459,
            0x11d2,
            {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
    struct EFI_GUID dpttp_guid = {
            0x8b843e20,
            0x8132,
            0x4852,
            {0x90, 0xcc, 0x55, 0x1a, 0x4e, 0x4a, 0x7f, 0x1c}};
    struct EFI_GUID dpftp_guid = {
            0x5c99a21,
            0xc70f,
            0x4ad2,
            {0x8a, 0x5f, 0x35, 0xdf, 0x33, 0x43, 0xf5, 0x1e}};
    // Watch Dog Timer. Default timer is 5 min.
    ST->BootServices->SetWatchdogTimer(/*Timeout=*/0, /*WatchDogCode=*/0,
                                       /*DataSize=*/0, /*WatchDogData*/ NULL);

    ST->BootServices->LocateProtocol(&gop_guid, NULL, (void **) &GOP);
    ST->BootServices->LocateProtocol(&spp_guid, NULL, (void **) &SPP);
    ST->BootServices->LocateProtocol(&sfsp_guid, NULL, (void **) &SFSP);
    ST->BootServices->LocateProtocol(&dpttp_guid, NULL, (void **) &DPTTP);
    ST->BootServices->LocateProtocol(&dpftp_guid, NULL, (void **) &DPFTP);
}

void putc(unsigned short c) {
    // One character is 2-byte in UEFI.
    unsigned short str[2] = L"";
    str[0] = c;
    ST->ConOut->OutputString(ST->ConOut, str);
}

void put(unsigned short *str) { ST->ConOut->OutputString(ST->ConOut, str); }

void puth(unsigned long long val, unsigned char len) {
    int i;
    unsigned short unicode;
    unsigned short str[100];

    for (i = len - 1; i >= 0; i--) {
        unicode = (unsigned short) (val & 0x0f);
        if (unicode < 0xa) {
            str[i] = L'0' + unicode;
        } else {
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
        ST->BootServices->WaitForEvent(/*NumberOfEvents=*/1,
                                       &(ST->ConIn->WaitForKey), &waitidx);
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
