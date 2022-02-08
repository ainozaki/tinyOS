#include "efi.h"

#ifndef FILE_H_
#define FILE_H_

#define MAX_FILE_NAME_LEN 8
#define MAX_FILE_NUM 10
#define MAX_FILE_BUF 1024
#define FILE_INFO_BUF_SIZE 180

#define EFI_FILE_MODE_READ 0x0000000000000001
#define EFI_FILE_MODE_WRITE 0x0000000000000002
#define EFI_FILE_MODE_CREATE 0x8000000000000000

unsigned long long sizeof_file(struct EFI_FILE_PROTOCOL *file);

void file_read(struct EFI_FILE_PROTOCOL *src,
               void *dst,
               unsigned long long size);


#endif// FILE_H_
