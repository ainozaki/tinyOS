#include "kbc.h"

#include "intr.h"
#include "pic.h"
#include "print.h"
#include "x86.h"

#define KBC_DATA_ADDR 0x0060
#define KBC_STATUS_ADDR 0x0064
#define KBC_DATA_IS_BREAK 0x80
#define KBC_STATUS_OBF 0x01

#define ASCII_ESC 0x1b
#define ASCII_BS 0x08
#define ASCII_HT 0x09

#define KBC_INTR_NO 33

const char keymap[] = {
        0x00, ASCII_ESC, '1',  '2',  '3',      '4',      '5',  '6',  '7',  '8',
        '9',  '0',       '-',  '^',  ASCII_BS, ASCII_HT, 'q',  'w',  'e',  'r',
        't',  'y',       'u',  'i',  'o',      'p',      '@',  '[',  '\n', 0x00,
        'a',  's',       'd',  'f',  'g',      'h',      'j',  'k',  'l',  ';',
        ':',  0x00,      0x00, ']',  'z',      'x',      'c',  'v',  'b',  'n',
        'm',  ',',       '.',  '/',  0x00,     '*',      0x00, ' ',  0x00, 0x00,
        0x00, 0x00,      0x00, 0x00, 0x00,     0x00,     0x00, 0x00, 0x00, 0x00,
        0x00, '7',       '8',  '9',  '-',      '4',      '5',  '6',  '+',  '1',
        '2',  '3',       '0',  '.',  0x00,     0x00,     0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,      0x00, 0x00, 0x00,     0x00,     0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,      0x00, 0x00, 0x00,     0x00,     0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,      0x00, 0x00, 0x00,     '_',      0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,      0x00, 0x00, 0x00,     '\\',     0x00, 0x00};

void do_kbc_interrupt() {
  // If OBF is off, return
  if (!(io_read(KBC_STATUS_ADDR) & KBC_STATUS_OBF)) {
    goto exit;
  }

  // If Make mode, return
  unsigned char key_data = io_read(KBC_DATA_ADDR);
  if (key_data & KBC_DATA_IS_BREAK) {
    goto exit;
  }

  unsigned char key = keymap[key_data];
  if (key >= 'a' && key <= 'z') {
    key = key - 'a' + 'A';
  } else if (key == '\n') {
    putc('\r');
  }
  putc(key);

exit:
  // Inform PIC the end of interrupt.
  set_pic_eoi(KBC_INTR_NO);
}

void kbc_handler();

void kbc_intr_init() {
  void *handler;
  __asm__ volatile("lea kbc_handler, %[handler]" : [handler] "=r"(handler));
  set_intr(KBC_INTR_NO, handler);
  enable_pic_intr(KBC_INTR_NO);
}
