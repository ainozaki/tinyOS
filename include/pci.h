#ifndef PCI_H_
#define PCI_H_

#define PCI_CONF_DID_VID 0x00
#define PCI_CONF_CLASS 0x08
#define PCI_CONF_HEADER 0x0c
#define PCI_CONF_BAR 0x10
#define CONFIG_ADDRESS 0x0cf8
#define CONFIG_DATA 0x0cfc

// BAR
#define PCI_BAR_MASK_IO 0x00000001
#define PCI_BAR_MASK_32 0x00000000
#define PCI_BAR_MASK_1MB 0x00000002
#define PCI_BAR_MASK_64 0x00000004
#define PCI_BAR_MASK_PREFETCH 0x00000008
#define PCI_BAR_MASK_MEMTYPE 0x00000006

// REG
#define NIC_REG_IMS 0x00d0
#define NIC_REG_IMC 0x00d8
#define NIC_REG_RCTL 0x0100
#define NIC_REG_RDBAL 0x2800
#define NIC_REG_RDBAH 0x2804
#define NIC_REG_RDLEN 0x2808
#define NIC_REG_RDH 0x2810
#define NIC_REG_RDT 0x2818

// Rxdesc
#define RXDESC_NUM 80
#define ALIGN_MARGIN 16

#define PACKET_BUFFER_SIZE 1024
#define PACKET_RBSIZE_BIT (0b01 << 16)

// RCTL
#define NIC_RCTL_EN (1U << 1)
#define NIC_RCTL_SBP (1U << 2)
#define NIC_RCTL_UPE (1U << 3)
#define NIC_RCTL_MPE (1U << 4)
#define NIC_RCTL_BAM (1U << 15)

#define NIC_RDESC_STAT_DD (1U << 0)

union pci_config_address {
  unsigned int raw;
  struct __attribute__((packed)) {
    unsigned int reg_addr : 8;
    unsigned int func_num : 3;
    unsigned int dev_num : 5;
    unsigned int bus_num : 8;
    unsigned int _reserved : 7;
    unsigned int enable_bit : 1;
  };
};

struct __attribute__((packed)) rxdesc {
  unsigned long long buffer_addr;
  unsigned short length;
  unsigned short packet_checksum;
  unsigned char status;
  unsigned char errors;
  unsigned short special;
};

void nic_init();

void rx_init();

unsigned int get_pci_conf(unsigned char bus,
                          unsigned char dev,
                          unsigned char func,
                          unsigned char reg);
void pci_search_and_dump();

void get_nic_bar();


unsigned int get_nic_reg(unsigned short reg);
void set_nic_reg(unsigned short reg, unsigned int value);

void dump_nic_ims();

unsigned short dump_frame();

#endif// PCI_H_
