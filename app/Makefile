TARGET = test
CFLAGS = -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common \
-Iinclude -fPIE
LDFLAGS = -Map app.map -s -x -T ./app.ld -pie
OBJS = app.o lib.o

$(TARGET): $(OBJS)
	ld $(LDFLAGS) -o $@ $+

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

deploy: $(TARGET)
	../create_fs.sh $(TARGET)
	rm -f ../fs/fs.img
	mv fs.img ../fs/

clean:
	rm -f *~ *.o *.map $(TARGET)

.PHONY: deploy clean
