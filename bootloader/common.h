#ifndef COMMON_H_
#define COMMON_H_

#define NULL (void *) 0
#define TRUE 1
#define FALSE 0

void assert(unsigned long long status, unsigned short *message);

void put(unsigned short *str);

void put_param(unsigned short *name, unsigned long long val);

void putc(unsigned short c);

void puth(unsigned long long val, unsigned char len);

void put_n_bytes(unsigned char *addr, unsigned int num);

#endif// COMMON_H_
