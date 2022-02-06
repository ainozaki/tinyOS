#ifndef EFI_H_
#define EFI_H_

// used in EFI_SIMPLE_TEXT_INPUT_PROTOCOL.ReadKeyStroke
struct EFI_INPUT_KEY {
    unsigned short ScanCode;
    unsigned short UnicodeChar;
};

// Pixel format.
// use EFI_GRAPHICS_PIXEL_FORMAT.PixelBlueGreenRedReserved8BitPerColor format
// for QEMU OVMF.
struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL {
    unsigned char Blue;
    unsigned char Green;
    unsigned char Red;
    unsigned char Reserved;
};

// used in SystemTable.BootServices.LocateProtocol
struct EFI_GUID {
    unsigned int Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
};

// for GUI
struct EFI_GRAPHICS_OUTPUT_PROTOCOL {
    unsigned long long _buf[3];
    struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE {
        unsigned int MaxMode;
        unsigned int Mode;
        struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION {
            unsigned int Version;
            unsigned int HorizontalResolution;
            unsigned int VerticalResolution;
            enum EFI_GRAPHICS_PIXEL_FORMAT {
                PixelRedGreenBlueReserved8BitPerColor,
                PixelBlueGreenRedReserved8BitPerColor,
                PixelBitMask,
                PixelBltOnly,
                PixelFormatMax
            } PixelFormat;
        } * Info;
        unsigned long long SizeOfInfo;
        unsigned long long FrameBufferBase;
        unsigned long long FrameBufferSize;
    } * Mode;
};

// for mouse
struct EFI_SIMPLE_POINTER_STATE {
    int RelativeMovementX;
    int RelativeMovementY;
    int RelativeMovementZ;
    unsigned char LeftButton;
    unsigned char RightButton;
};

struct EFI_SIMPLE_POINTER_PROTOCOL {
    unsigned long long (*Reset)(struct EFI_SIMPLE_POINTER_PROTOCOL *This,
                                unsigned char ExtendedVerification);

    unsigned long long (*GetState)(struct EFI_SIMPLE_POINTER_PROTOCOL *This,
                                   struct EFI_SIMPLE_POINTER_STATE *State);

    void *WaitForInput;
};

// for file
struct EFI_FILE_PROTOCOL {
    unsigned long long _buf;

    unsigned long long (*Open)(struct EFI_FILE_PROTOCOL *This,
                               struct EFI_FILE_PROTOCOL **NewHandle,
                               unsigned short *FileName,
                               unsigned long long OpenMode,
                               unsigned long long Attributes);

    unsigned long long (*Close)(struct EFI_FILE_PROTOCOL *This);

    unsigned long long _buf2;

    unsigned long long (*Read)(struct EFI_FILE_PROTOCOL *This,
                               unsigned long long *BufferSize, void *Buffer);

    unsigned long long (*Write)(struct EFI_FILE_PROTOCOL *This,
                                unsigned long long *BufferSize, void *Buffer);

    unsigned long long _buf3[2];

    unsigned long long (*GetInfo)(
            struct EFI_FILE_PROTOCOL *This,
            struct EFI_GUID *InformationType,
            unsigned long long *BufferSize,
            void *Buffer);

    unsigned long long _buf4;

    unsigned long long (*Flush)(struct EFI_FILE_PROTOCOL *This);
};

struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
    unsigned long long Revision;

    unsigned long long (*OpenVolume)(struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,
                                     struct EFI_FILE_PROTOCOL **Root);
};

struct EFI_TIME {
    unsigned short Year; // 1900 – 9999
    unsigned char Month; // 1 – 12
    unsigned char Day;   // 1 – 31
    unsigned char Hour;  // 0 – 23
    unsigned char Minute;// 0 – 59
    unsigned char Second;// 0 – 59
    unsigned char Pad1;
    unsigned int Nanosecond;// 0 – 999,999,999
    unsigned short TimeZone;// -1440 to 1440 or 2047
    unsigned char Daylight;
    unsigned char Pad2;
};

struct EFI_FILE_INFO {
    unsigned long long Size;
    unsigned long long FileSize;
    unsigned long long PhysicalSize;
    struct EFI_TIME CreateTime;
    struct EFI_TIME LastAccessTime;
    struct EFI_TIME ModificationTime;
    unsigned long long Attribute;
    unsigned short FileName[];
};

// for device path

// The 4th argument of OpenProtocol(): Attributes
#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL 0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL 0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL 0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER 0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE 0x00000020
struct EFI_DEVICE_PATH_PROTOCOL {
    unsigned char Type;
    unsigned char SubType;
    unsigned char Length[2];
};

enum EFI_MEMORY_TYPE {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiMaxMemoryType
};

struct EFI_MEMORY_DESCRIPTOR {
    unsigned int Type;
    unsigned long long PhysicalStart;
    unsigned long long VirtualStart;
    unsigned long long NumberOfPages;
    unsigned long long Attribute;
};

struct EFI_LOADED_IMAGE_PROTOCOL {
    unsigned int Revision;
    void *ParentHandle;
    struct EFI_SYSTEM_TABLE *SystemTable;
    void *DeviceHandle;
    struct EFI_DEVICE_PATH_PROTOCOL *FilePath;
    void *Reserved;
    unsigned int LoadOptionsSize;
    void *LoadOptions;
    void *ImageBase;
    unsigned long long ImageSize;
    enum EFI_MEMORY_TYPE ImageCodeType;
    enum EFI_MEMORY_TYPE ImageDataType;

    unsigned long long (*Unload)(void *ImageHandle);
};

// Convert device path to text
struct EFI_DEVICE_PATH_TO_TEXT_PROTOCOL {
    unsigned long long _buf;

    unsigned short *(*ConvertDevicePathToText)(
            const struct EFI_DEVICE_PATH_PROTOCOL *DeviceNode,
            unsigned char DisplayOnly, unsigned char AllowShortcuts);
};

// Convert text to device path
struct EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL {
    unsigned long long _buf;

    struct EFI_DEVICE_PATH_PROTOCOL *(*ConvertTextToDevicePath)(
            const unsigned short *TextDevicePath);
};

struct EFI_DEVICE_PATH_UTILITIES_PROTOCOL {
    unsigned long long _buf[3];

    struct EFI_DEVICE_PATH_PROTOCOL *(*AppendDeviceNode)(
            const struct EFI_DEVICE_PATH_PROTOCOL *DevicePath,
            const struct EFI_DEVICE_PATH_PROTOCOL *DeviceNode);
};


// SystemTable is an argument of the UEFI's entry function.
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
        // Task Priority
        unsigned long long _buf2[2];
        // Memory
        unsigned long long _buf3[2];

        unsigned long long (*GetMemoryMap)(unsigned long long *MemoryMapSize, struct EFI_MEMORY_DESCRIPTOR *MemoryMap,
                                           unsigned long long *MapKey, unsigned long long *DescriptorSize,
                                           unsigned int *DescriptorVersion);
        unsigned long long _buf3_2[2];


        // Event and timer
        unsigned long long _buf4[2];

        unsigned long long (*WaitForEvent)(unsigned long long NumberOfEvents,
                                           void **Event, unsigned long long *Index);

        unsigned long long _buf4_2[3];

        // Protocol Handler
        unsigned long long _buf5[9];

        // Image
        unsigned long long (*LoadImage)(unsigned char BootPolicy, void *ParentImageHandle, struct EFI_DEVICE_PATH_PROTOCOL *DevicePath,
                                        void *SourceBuffer, unsigned long long SourceSize, void **ImageHandle);

        unsigned long long (*StartImage)(void *ImageHandle, unsigned long long *ExitDataSize, unsigned short **ExitData);

        unsigned long long _buf6[2];

        unsigned long long (*ExitBootServices)(void *ImageHandle, unsigned long long MapKey);

        // Miscellaneous
        unsigned long long _buf7[2];

        unsigned long long (*SetWatchdogTimer)(unsigned long long Timeout,
                                               unsigned long long WatchdogCode,
                                               unsigned long long DataSize,
                                               unsigned short *WatchdogData);

        // Driver Support
        unsigned long long _buf8[2];

        // Open/Close Protocol
        unsigned long long (*OpenProtocol)(void *Handle, struct EFI_GUID *Protocol,
                                           void **Interface, void *AgentHandle,
                                           void *ControllerHandle,
                                           unsigned int Attributes);

        unsigned long long _buf9[2];

        // Library
        unsigned long long _buf10[2];

        unsigned long long (*LocateProtocol)(struct EFI_GUID *Protocol,
                                             void *Registration, void **Interface);

        unsigned long long _buf10_2[2];

        // 32-bit CRC
        unsigned long long _buf11;

        // Miscellaneous
        void (*CopyMem)(void *Destination, void *Source, unsigned long long Length);

        void (*SetMem)(void *Buffer, unsigned long long Size, unsigned char Value);

        unsigned long long _buf12;

    } * BootServices;
};

extern struct EFI_SYSTEM_TABLE *ST;
extern struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;
extern struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;

void efi_init(struct EFI_SYSTEM_TABLE *SystemTable);

void exit_boot_services(void *ImageHandle);

#endif// EFI_H_
