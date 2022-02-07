#include "kbc.h"

#include "x86.h"

#define KBC_DATA_ADDR 0x0060
#define KBC_STATUS_ADDR 0x0064
#define KBC_DATA_MAKE_BREAK 0x80
#define KBC_STATUS_OBF 0x01

#define ASCII_ESC 0x1b 
#define ASCII_BS 0x08
#define ASCII_HT 0x09

const char keymap[] = {
	0x00, ASCII_ESC, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '^', ASCII_BS, ASCII_HT,
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '@', '[', '\n', 0x00, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	':', 0x00, 0x00, ']', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0x00, '*',
	0x00, ' ', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '7',
	'8', '9', '-', '4', '5', '6', '+', '1',
	'2', '3', '0', '.', 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, '_', 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, '\\', 0x00, 0x00
};

unsigned char get_kbc(){
	// wait until KBC_STATUS_OBF is set.
	while (!(io_read(KBC_STATUS_ADDR) & KBC_STATUS_OBF));
	return io_read(KBC_DATA_ADDR);
}

unsigned char get_keycode() {
	unsigned char value;
	// wait until KBC_DATA_MAKE_BREAK is MAKE(=0).
	while ((value = get_kbc()) & KBC_DATA_MAKE_BREAK);
	return value;
}

unsigned char get_c(){
	return keymap[get_keycode()];
}
