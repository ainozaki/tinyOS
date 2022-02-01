SRC = main.c
TARGET = fs/EFI/BOOT/BOOTX64.EFI

all: $(TARGET)

$(TARGET): $(SRC)
	x86_64-w64-mingw32-gcc -Wall -Wextra -e efi_main -nostdinc -nostdlib -fno-builtin -Wl,--subsystem,10 -o $(TARGET) $(SRC)

run:
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -hda fat:rw:fs

clean:
	rm -rf $(TARGET)

.PHONY: run clean