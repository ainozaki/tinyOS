TARGET = kernel.bin
CFLAGS = -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common
LDFLAGS = -Map kernel.map -s -x -T kernel.ld

$(TARGET): main.o
	ld $(LDFLAGS) -o $@ $+
%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	cp $(TARGET) fs/
	#qemu-system-x86_64 -bios OVMF.fd -hda fat:rw:fs
	qemu-system-x86_64 -bios OVMF.fd -drive if=ide,index=0,media=disk,format=raw,file=fat:rw:fs
clean:
	rm -f *- *.o *.map $(TARGET)

.PHONY: clean

