#include "common.h"

#include "efi.h"

void assert(unsigned long long status, unsigned short *message) {
    if (status) {
        put(message);
        put(L": ");
        puth(status, 16);
        put(L"\r\n");
        // If status isn't FALSE, stop here.
        while (1)
            ;
    }
}