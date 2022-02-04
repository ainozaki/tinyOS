#include "common.h"

#include "efi.h"

void assert(unsigned long long status, unsigned short *message) {
    put(message);
    put(L": ");
    puth(status, 16);
    put(L"\r\n");
}