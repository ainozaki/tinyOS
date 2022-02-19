#include "pci.h"

#include "common.h"
#include "print.h"
#include "x86.h"

unsigned char nic_bus;
unsigned char nic_dev;
unsigned char nic_func;

// Buffer for Ethernet frame
static unsigned char rx_buffer[RXDESC_NUM][PACKET_BUFFER_SIZE];
// Buffer for rxdesc
static unsigned char
        rxdesc_data[(sizeof(struct rxdesc) * RXDESC_NUM) + ALIGN_MARGIN];
static struct rxdesc *rxdesc_base;
static unsigned short current_rx_idx;

unsigned int nic_reg_base;

void nic_init() {
  pci_search_and_dump();

  get_nic_bar();

  rx_init();
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

void rx_init() {
  // rxdesc base addr should be 16byte align
  unsigned long long rxdesc_addr = (unsigned long long) rxdesc_data;
  rxdesc_addr = (rxdesc_addr + ALIGN_MARGIN) & 0xfffffffffffffff0;
  rxdesc_base = (struct rxdesc *) rxdesc_addr;

  // Initialize rxdesc
  struct rxdesc *current_rxdesc = rxdesc_base;
  int i;
  for (i = 0; i < RXDESC_NUM; i++) {
    current_rxdesc->buffer_addr = (unsigned long long) rx_buffer[i];
    current_rxdesc->status = 0;
    current_rxdesc->errors = 0;
    current_rxdesc++;
  }

  // Register rxdesc info to NIC reg
  set_nic_reg(NIC_REG_RDBAH, rxdesc_addr >> 32);
  set_nic_reg(NIC_REG_RDBAL, rxdesc_addr & 0x00000000ffffffff);
  set_nic_reg(NIC_REG_RDLEN, sizeof(struct rxdesc) * RXDESC_NUM);

  // Set rxdesc Head and Tail
  current_rx_idx = 0;
  set_nic_reg(NIC_REG_RDH, current_rx_idx);
  set_nic_reg(NIC_REG_RDT, RXDESC_NUM - 1);

  // Rx setting
  set_nic_reg(NIC_REG_RCTL, PACKET_RBSIZE_BIT | NIC_RCTL_BAM | NIC_RCTL_MPE |
                                    NIC_RCTL_UPE | NIC_RCTL_SBP | NIC_RCTL_EN);
}

unsigned short receive_frame(void *buf) {
  unsigned short len = 0;
  struct rxdesc *cur_rxdesc = rxdesc_base + current_rx_idx;

  if (cur_rxdesc->status & NIC_RDESC_STAT_DD) {
    len = cur_rxdesc->length;
    memcpy(buf, (void *) cur_rxdesc->buffer_addr, cur_rxdesc->length);

    cur_rxdesc->status = 0;

    set_nic_reg(NIC_REG_RDT, current_rx_idx);

    current_rx_idx = (current_rx_idx + 1) % RXDESC_NUM;
  }

  return len;
}

unsigned short dump_frame() {
  unsigned char buf[PACKET_BUFFER_SIZE];
  unsigned short len;
  len = receive_frame(buf);

  unsigned short i;
  for (i = 0; i < len; i++) {
    puth_without_0x(buf[i], 2);

    if (((i + 1) % 16) == 0) {
      puts("\r\n");
    } else if (((i + 1) % 2) == 0) {
      puts(" ");
    }
  }
  if (len > 0) {
    puts("\r\n");
  }
  return len;
}
