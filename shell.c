#include "shell.h"

#include "common.h"
#include "efi.h"
#include "graphics.h"
#include "gui.h"
#include "string.h"

#define MAX_CMDLINE 32

void pstat(void){
    unsigned long long status;
    struct EFI_SIMPLE_POINTER_STATE s;
    unsigned long long waitidx;

    SPP->Reset(SPP, /*ExtendedVerification=*/FALSE);
    while (1) {
        ST->BootServices->WaitForEvent(1, &(SPP->WaitForInput), &waitidx);
        status = SPP->GetState(SPP, &s);
        if (!status){
            puth(s.RelativeMovementX, 8);
            put(L" ");
            puth(s.RelativeMovementY, 8);
            put(L" ");
            puth(s.RelativeMovementZ, 8);
            put(L" ");
            puth(s.LeftButton, 1);
            put(L" ");
            puth(s.RightButton, 1);
            put(L" ");
        }
    }
}

void shell(void) {
  unsigned short cmdline[MAX_CMDLINE];

  while (1) {
    put(L"inos> ");

    wait_and_get_line(cmdline, MAX_CMDLINE);
    if (strncmp(cmdline, L"hello", 6) == 0) {
      put(L"hello\r\n");
    } else if (strncmp(cmdline, L"gui", 4) == 0) {
      gui();
    } else if (strncmp(cmdline, L"pstat", 6) == 0){
        pstat();
    }else {
      put(L"command not found.\r\n");
    }
  }
}
