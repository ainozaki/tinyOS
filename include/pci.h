#ifndef PCI_H_
#define PCI_H_

#define PCI_CONF_DID_VID 0x00
#define PCI_CONF_CLASS 0x08
#define PCI_CONF_HEADER 0x0c
#define CONFIG_ADDRESS 0x0cf8
#define CONFIG_DATA 0x0cfc


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

void pci_search_and_dump();

#endif// PCI_H_
