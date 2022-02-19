#ifndef COMMON_H_
#define COMMON_H_

#define NULL (void *) 0
#define FALSE 0
#define TRUE 1

void check_nullptr(void *ptr, char *message);

int strcmp(const char *s1, const char *s2);
int strncmp(char *s1, char *s2, unsigned long long n);

void memcpy(void *dst, void *src, unsigned long long size);

#endif// COMMON_H_
