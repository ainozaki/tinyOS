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

void put(unsigned short *str) { ST->ConOut->OutputString(ST->ConOut, str); }

void put_param(unsigned short *name, unsigned long long val) {
    put(name);
    put(L": 0x");
    puth(val, 16);
    put(L"\r\n");
}

void putc(unsigned short c) {
    // One character is 2-byte in UEFI.
    unsigned short str[2] = L"";
    str[0] = c;
    ST->ConOut->OutputString(ST->ConOut, str);
}


void puth(unsigned long long val, unsigned char len) {
    int i;
    unsigned short unicode;
    unsigned short str[100];

    for (i = len - 1; i >= 0; i--) {
        unicode = (unsigned short) (val & 0x0f);
        if (unicode < 0xa) {
            str[i] = L'0' + unicode;
        } else {
            str[i] = L'A' + (unicode - 0xa);
        }
        val >>= 4;
    }
    str[len] = L'\0';
    put(str);
}

void put_n_bytes(unsigned char *addr, unsigned int num) {
    unsigned int i;
    for (i = 0; i < num; i++) {
        puth(*addr++, 2);
        putc(L' ');
    }
}
