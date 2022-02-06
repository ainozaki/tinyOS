SRC = bootloader.c \
			common.c \
			efi.c \
			file.c
HEADER = common.h \
				 efi.h \
				 file.h
TARGET = BOOTX64.EFI

all: $(TARGET)
app: fs/test.efi

$(TARGET): $(SRC) $(HEADER)
	x86_64-w64-mingw32-gcc -Wall -Wextra -e efi_main -nostdinc -nostdlib -fno-builtin -Wl,--subsystem,10 -o $@ $(SRC)

fs/test.efi: testappp.c
	x86_64-w64-mingw32-gcc -Wall -Wextra -e efi_main -nostdinc -nostdlib -fno-builtin -Wl,--subsystem,10 -shared -o $@ $<
run:
	rm -f ../fs/EFI/BOOT/BOOTX64.EFI
	cp BOOTX64.EFI ../fs/EFI/BOOT/
	qemu-system-x86_64 -bios OVMF.fd -drive if=ide,index=0,media=disk,format=raw,file=fat:rw:../fs

clean:
	rm -rf $(TARGET) fs/EFI/BOOT/BOOTX64.EFI

format: $(SRC) $(HEADER)
	clang-format -i $^

.PHONY: run clean format
