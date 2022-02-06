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
        status = ST->BootServices->GetMemoryMap(&mmap_size, (struct EFI_MEMORY_DESCRIPTOR *) mem_desc, &map_key,
                                                &mem_desc_unit_size, &desc_ver);
    } while (status);

    status = ST->BootServices->ExitBootServices(ImageHandle, map_key);
    assert(status, L"ExitBootServices");
}
