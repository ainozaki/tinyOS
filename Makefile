TARGET = kernel.bin
CFLAGS = -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common
LDFLAGS = -Map kernel.map -s -x -T kernel.ld

SRCS = turos.c \
			 acpi.c \
			 common.c \
			 font.c \
			 hpet.c \
			 intr.c \
			 kbc.c \
			 pic.c \
			 pixel.c \
			 print.c \
			 x86.c
SRCS_OBJ = $(SRCS:.c=.o) handler.o
HEADERS = acpi.h \
					common.h \
					font.h \
					hpet.h \
					intr.h \
					kbc.h \
					pic.h \
					pixel.h \
					print.h \
					x86.h

$(TARGET): $(SRCS_OBJ)
	ld $(LDFLAGS) -o $@ $+

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

%.o: %.s
	gcc $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	cp $(TARGET) fs/
	qemu-system-x86_64 -m 4G\
		-drive if=pflash,format=raw,readonly,file=/usr/share/OVMF/OVMF_CODE.fd \
		-drive if=pflash,format=raw,file=/usr/share/OVMF/OVMF_VARS.fd \
		-drive if=ide,index=0,media=disk,format=raw,file=fat:rw:fs

format:
	clang-format -i $(SRCS) $(HEADERS)

clean:
	rm -f *- *.o *.map $(TARGET) $(SRCS_OBJ)

.PHONY: clean

