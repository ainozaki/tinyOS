#include "pci.h"

#include "print.h"
#include "x86.h"

void pci_search_and_dump() {
  int bus = 0;
  int dev = 0;
  // Bus 0-255
  for (bus = 0; bus < 256; bus++) {
    union pci_config_address conf_addr;
    conf_addr.raw = 0;
    // Device 0-31
    for (dev = 0; dev < 32; dev++) {
      // Read function 0
      conf_addr.bus_num = bus;
      conf_addr.dev_num = dev;
      conf_addr.func_num = 0;
      conf_addr.reg_addr = PCI_CONF_DID_VID;
      conf_addr.enable_bit = 1;
      io_write32(CONFIG_ADDRESS, conf_addr.raw);
      unsigned int conf_data = io_read32(CONFIG_DATA);
      unsigned short vendor_id = conf_data & 0x0000ffff;
      unsigned short device_id = conf_data >> 16;

      if (vendor_id != 0xffff) {
        // Read class
        conf_addr.reg_addr = PCI_CONF_CLASS;
        conf_addr.enable_bit = 1;
        io_write32(CONFIG_ADDRESS, conf_addr.raw);
        unsigned int class = io_read32(CONFIG_DATA);
        unsigned char interface = class >> 8;
        unsigned char sub_class = class >> 16;
        unsigned char base_class = class >> 24;

        // Read header_type
        conf_addr.reg_addr = PCI_CONF_HEADER;
        conf_addr.enable_bit = 1;
        io_write32(CONFIG_ADDRESS, conf_addr.raw);
        unsigned int header = io_read32(CONFIG_DATA);
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
