#include "shell.h"

#include "efi.h"
#include "string.h"

#define MAX_CMDLINE 32

void shell(void) {
  while (1) {
    put(L"inos> ");

    unsigned short cmdline[MAX_CMDLINE];
    wait_and_get_line(cmdline, MAX_CMDLINE);
    if (strncmp(cmdline, L"hello", 6) == 0) {
      put(L"hello\r\n");
    } else {
      put(L"command not found.\r\n");
    }
  }
}
