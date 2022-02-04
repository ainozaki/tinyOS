#include "common.h"
#include "efi.h"
#include "shell.h"

void efi_main(void *ImageHandle __attribute__((unused)),
              struct EFI_SYSTEM_TABLE *SystemTable) {
    unsigned long long status;
    struct EFI_LOADED_IMAGE_PROTOCOL *lip;
    struct EFI_DEVICE_PATH_PROTOCOL *dev_path;
    void *image;

    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    efi_init(SystemTable);

    // Get ImageHandle's EFI_LOADED_IMAGE_PROTOCOL
    status = ST->BootServices->OpenProtocol(ImageHandle, &lip_guid, (void **) &lip,
                                            ImageHandle, /*ControllerHandle=*/
                                            NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    assert(status, L"OpenProtocol(lip)");
    put(L"lip->FilePath: ");
    put(DPTTP->ConvertDevicePathToText(lip->FilePath, /*DisplayOnly=*/FALSE,
            /*AllowShortcuts=*/FALSE));
    put(L"\r\n");

    // Get lip->DeviceHandle's EFI_DEVICE_PATH_PROTOCOL
    status = ST->BootServices->OpenProtocol(lip->DeviceHandle, &dpp_guid, (void **) &dev_path,
                                            ImageHandle, /*ControllerHandle=*/NULL,
                                            EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    assert(status, L"OpenProtocol(dpp)");
    put(L"dev_path: ");
    put(DPTTP->ConvertDevicePathToText(dev_path, /*DisplayOnly=*/FALSE,
            /*AllowShortcut=*/FALSE));
    put(L"\r\n");

    status = ST->BootServices->LoadImage(/*BootPolicy=*/FALSE, ImageHandle, dev_path, /*SourceBuffer=*/
                                                        NULL, /*SourceSize=*/0, &image);
    assert(status, L"LoadImage");
    put(L"LoadImage: Success\r\n");
    shell();
}
