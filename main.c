#include "common.h"
#include "efi.h"
#include "shell.h"

void efi_main(void *ImageHandle __attribute__((unused)),
              struct EFI_SYSTEM_TABLE *SystemTable) {
    unsigned long long status;
    struct EFI_LOADED_IMAGE_PROTOCOL *lip;
    struct EFI_DEVICE_PATH_PROTOCOL *dev_path;

    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    efi_init(SystemTable);

    status = ST->BootServices->OpenProtocol(ImageHandle, &lip_guid, (void **) &lip,
                                            ImageHandle, /*ControllerHandle=*/
                                            NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    assert(status, L"OpenProtocol");

    put(L"lip->FilePath: ");
    put(DPTTP->ConvertDevicePathToText(lip->FilePath, /*DisplayOnly=*/FALSE,
                                       /*AllowShortcuts=*/FALSE));
    put(L"\r\n");

    dev_path = DPFTP->ConvertTextToDevicePath(L"\\test.efi");
    put(L"dev_path: ");
    put(DPTTP->ConvertDevicePathToText(dev_path, /*DisplayOnly=*/FALSE,
                                       /*AllowShortcut=*/FALSE));
    put(L"\r\n");

    shell();
}
