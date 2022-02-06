TARGET = kernel.bin
CFLAGS = -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common
LDFLAGS = -Map kernel.map -s -x -T kernel.ld

SRCS = turos.c \
			 pixel.c
SRCS_OBJ = $(SRCS:.c=.o)
HEADERS = pixel.h

$(TARGET): $(SRCS_OBJ)
	@echo "+is" $+
	ld $(LDFLAGS) -o $@ $+

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	cp $(TARGET) fs/
	qemu-system-x86_64 -bios OVMF.fd -drive if=ide,index=0,media=disk,format=raw,file=fat:rw:fs

format:
	clang-format -i $(SRCS) $(HEADERS)

clean:
	rm -f *- *.o *.map $(TARGET) $(SRCS_OBJ)

.PHONY: clean

