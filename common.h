#ifndef COMMON_H_
#define COMMON_H_

#define NULL (void *) 0
#define FALSE 0
#define TRUE 1

void check_nullptr(void *ptr, char *message);

int strncmp(char *s1, char *s2, unsigned long long n);

#endif// COMMON_H_
