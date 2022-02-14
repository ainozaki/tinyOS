#include "common.h"

#include "print.h"

void check_nullptr(void *ptr, char *message) {
  if (!ptr) {
    puts("ASSERT ");
    puts(message);
    // If status isn't FALSE, stop here.
    while (1)
      ;
  }
}

int strcmp(const char *s1, const char *s2) {
  unsigned char c1, c2;
  int res = 0;

  do {
    c1 = *s1++;
    c2 = *s2++;
    res = c1 - c2;
    if (res) {
      break;
    }
  } while (c1);
  return res;
}

int strncmp(char *s1, char *s2, unsigned long long n) {
  unsigned long long i;
  for (i = 0; i < n; i++) {
    if (*s1 != *s2) {
      return 1;
    }
    if ((*s1 == '\0') | (*s2 == '\0')) {
      return 1;
    }
    s1++;
    s2++;
  }
  return 0;
}
