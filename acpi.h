#ifndef ACPI_H_
#define ACPI_H_

struct RSDP {
  char Signature[8];
  unsigned char Checksum;
  char OEMID[6];
  unsigned char Revision;
  unsigned int RsdtAddress;
  unsigned int Length;
  unsigned long long XsdtAddress;
  unsigned char Extended_Checksum;
  unsigned char Reserved[3];
};

struct SDTH {
  char Signature[4];
  unsigned int Length;
  unsigned char Revision;
  unsigned char Checksum;
  char OEMID[6];
  char OEM_Table_ID[8];
  unsigned int OEM_Revision;
  unsigned int Creator_ID;
  unsigned int Creator_Revision;
};

struct __attribute__((packed)) XSDT {
  struct SDTH Header;
  struct SDTH *Entry[0];
};

void init_acpi(void *rsdp);

void dump_xsdt_entries();
void dump_sdth_signature(struct SDTH *sdth);

struct SDTH *get_sdth(char *signature);

#endif// ACPI_H_
