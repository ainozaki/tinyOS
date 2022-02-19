#include "pci.h"

#include "print.h"
#include "x86.h"

unsigned char nic_bus;
unsigned char nic_dev;
unsigned char nic_func;

unsigned int nic_reg_base;

void nic_init() {
  pci_search_and_dump();

  get_nic_bar();
}

unsigned int get_pci_conf(unsigned char bus,
                          unsigned char dev,
                          unsigned char func,
                          unsigned char reg) {
  union pci_config_address conf_addr;
  conf_addr.raw = 0;
  conf_addr.bus_num = bus;
  conf_addr.dev_num = dev;
  conf_addr.func_num = func;
  conf_addr.reg_addr = reg;
  conf_addr.enable_bit = 1;
  io_write32(CONFIG_ADDRESS, conf_addr.raw);

  return io_read32(CONFIG_DATA);
}

void pci_search_and_dump() {
  int bus = 0;
  int dev = 0;
  // Bus 0-255
  for (bus = 0; bus < 256; bus++) {
    // Device 0-31
    for (dev = 0; dev < 32; dev++) {
      unsigned int conf_data =
              get_pci_conf(bus, dev, /*func=*/0, PCI_CONF_DID_VID);
      unsigned short vendor_id = conf_data & 0x0000ffff;
      unsigned short device_id = conf_data >> 16;

      if (vendor_id != 0xffff) {
        // Read class
        unsigned int class = get_pci_conf(bus, dev, /*func=*/0, PCI_CONF_CLASS);
        unsigned char interface = class >> 8;
        unsigned char sub_class = class >> 16;
        unsigned char base_class = class >> 24;

        // Find Ethernet controller
        if (base_class == 0x02) {
          if (sub_class == 0x00) {
            if (interface == 0x00) {
              nic_bus = bus;
              nic_dev = dev;
              nic_func = 0;
            }
          }
        }

        // Read header_type
        unsigned int header =
                get_pci_conf(bus, dev, /*func=*/0, PCI_CONF_HEADER);
        unsigned char header_type = header >> 16;

        puts("VENDOR ID: ");
        puth(vendor_id, 4);
        puts(" DEVICE ID: ");
        puth(device_id, 4);
        puts(" CLASS: ");
        puth(base_class, 2);
        puts(" ");
        puth(sub_class, 2);
        puts(" ");
        puth(interface, 2);
        puts(" HEADER_TYPE:7: ");
        puth(header_type & 0x80, 1);
        puts("\r\n");
      }
    }
  }
}

void get_nic_bar() {
  unsigned int bar = get_pci_conf(nic_bus, nic_dev, nic_func, PCI_CONF_BAR);
  puts("BAR: ");
  puth(bar, 8);
  puts(" ");

  if (bar & PCI_BAR_MASK_IO) {
    puts("IO BASE\r\n");
  } else {
    switch (bar & PCI_BAR_MASK_MEMTYPE) {
      case PCI_BAR_MASK_32:
        puts("MEM BASE IS 32BIT: ");
        nic_reg_base = bar & 0xfffffff0;
        puth(nic_reg_base, 8);
        puts("\r\n");
        break;
      case PCI_BAR_MASK_1MB:
        puts("1MB");
        break;
      case PCI_BAR_MASK_64:
        puts("64BIT");
        break;
      default:
        break;
    }
  }
}

unsigned int get_nic_reg(unsigned short reg) {
  unsigned long long addr = nic_reg_base + reg;
  return *(volatile unsigned int *) addr;
}

void set_nic_reg(unsigned short reg, unsigned int value) {
  unsigned long long addr = nic_reg_base + reg;
  *(volatile unsigned int *) addr = value;
}

void dump_nic_ims() {
  unsigned int ims = get_nic_reg(NIC_REG_IMS);
  puts("IMS: ");
  puth(ims, 8);
  puts("\r\n");
}
