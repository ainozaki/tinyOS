#include "common.h"
#include "efi.h"
#include "shell.h"

void efi_main(void *ImageHandle __attribute__((unused)),
              struct EFI_SYSTEM_TABLE *SystemTable) {
    unsigned long long status;
    struct EFI_LOADED_IMAGE_PROTOCOL *lip;
    struct EFI_DEVICE_PATH_PROTOCOL *dev_path;
    struct EFI_DEVICE_PATH_PROTOCOL *dev_node;
    struct EFI_DEVICE_PATH_PROTOCOL *dev_path_merged;
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

    // Create test.efi's device node.
    dev_node = DPFTP->ConvertTextToDevicePath(L"test.efi");

    // Concatinate dev_path and dev_node
    dev_path_merged = DPUP->AppendDeviceNode(dev_path, dev_node);
    put(L"dev_path_merged: ");
    put(DPTTP->ConvertDevicePathToText(dev_path_merged, /*DisplayOnle=*/FALSE, /*AllowShortcut=*/FALSE));
    put(L"\r\n");

    // Load image
    status = ST->BootServices->LoadImage(/*BootPolicy=*/FALSE, ImageHandle, dev_path_merged, /*SourceBuffer=*/
                                         NULL, /*SourceSize=*/0, &image);
    assert(status, L"LoadImage");
    put(L"LoadImage: Success\r\n");

    // Start imgae
    status = ST->BootServices->StartImage(image, /*ExitDataSize=*/NULL, /*ExitData=*/NULL);
    assert(status, L"StartImage");
    put(L"StartImage: Success\r\n");

    shell();
}
