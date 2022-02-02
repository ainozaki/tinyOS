#include "string.h"

int strncmp(const unsigned short *s1, const unsigned short *s2,
            const unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        if (*s1 != *s2) {
            return 1;
        } else if (*s1 == L'\0' && *s2 == L'\0') {
            return 0;
        }
        s1++;
        s2++;
    }
    return 1;
}

void strncpy(unsigned short *dst, unsigned short *src, unsigned long long n) {
    while (n--) {
        *dst++ = *src++;
    }
}
