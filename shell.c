#include "shell.h"

#include "efi.h"
#include "graphics.h"
#include "string.h"

#define MAX_CMDLINE 32

void shell(void) {
  struct RECT r = {10, 10, 100, 200};
  unsigned short cmdline[MAX_CMDLINE];

  while (1) {
    put(L"inos> ");

    wait_and_get_line(cmdline, MAX_CMDLINE);
    if (strncmp(cmdline, L"hello", 6) == 0) {
      put(L"hello\r\n");
    } else if (strncmp(cmdline, L"gui", 4) == 0) {
      draw_rect(r, white);
    } else {
      put(L"command not found.\r\n");
    }
  }
}
