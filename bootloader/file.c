#include "file.h"

#include "common.h"
#include "efi.h"

#define SAFETY_READ_UNIT 16384// 16KB

unsigned long long sizeof_file(struct EFI_FILE_PROTOCOL *file) {
  unsigned long long status;
  unsigned long long fi_size = FILE_INFO_BUF_SIZE;
  unsigned long long fi_buf[FILE_INFO_BUF_SIZE];
  struct EFI_FILE_INFO *fi_ptr;
  struct EFI_GUID fi_guid = {0x09576e92,
                             0x6d3f,
                             0x11d2,
                             {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

  status = file->GetInfo(file, &fi_guid, &fi_size, fi_buf);
  assert(status, L"file->GetInfo");

  fi_ptr = (struct EFI_FILE_INFO *) fi_buf;

  return fi_ptr->FileSize;
}

void file_read(struct EFI_FILE_PROTOCOL *src,
               void *dst,
               unsigned long long size) {
  unsigned long long status;
  unsigned char *d = dst;

  while (size > SAFETY_READ_UNIT) {
    unsigned long long unit = SAFETY_READ_UNIT;
    status = src->Read(src, &unit, (void *) d);
    assert(status, L"Read");
    d += unit;
    size -= unit;
  }
  while (size > 0) {
    unsigned long long unit = size;
    status = src->Read(src, &unit, (void *) d);
    assert(status, L"Read");
    size -= unit;
  }
}
