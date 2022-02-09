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
