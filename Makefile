SRC = main.c \
			efi.c \
			shell.c \
			string.c
HEADER = efi.h \
					shell.h \
					string.h
TARGET = fs/EFI/BOOT/BOOTX64.EFI

all: $(TARGET)

$(TARGET): $(SRC) $(HEADER)
	x86_64-w64-mingw32-gcc -Wall -Wextra -e efi_main -nostdinc -nostdlib -fno-builtin -Wl,--subsystem,10 -o $@ $(SRC)

run:
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -hda fat:rw:fs

clean:
	rm -rf $(TARGET)

format: $(SRC) $(HEADER)
	clang-format -i $^

.PHONY: run clean format
