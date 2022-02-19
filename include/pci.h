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

void nic_init();

unsigned int get_pci_conf(unsigned char bus,
                          unsigned char dev,
                          unsigned char func,
                          unsigned char reg);
void pci_search_and_dump();

void get_nic_bar();


unsigned int get_nic_reg(unsigned short reg);
void set_nic_reg(unsigned short reg, unsigned int value);

void dump_nic_ims();
#endif// PCI_H_
