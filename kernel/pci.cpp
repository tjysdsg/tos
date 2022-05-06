/// \file https://wiki.osdev.org/PCI
#include <stdint.h>
#include "pci.h"
#include "port.h"
#include "kprintf.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC
#define PCI_INVALID_VENDOR_ID 0xFFFF
#define PCI_DEVICE_HEADER_TYPE_GENERAL 0x0
#define PCI_DEVICE_HEADER_TYPE_PCI_TO_PCI 0x1
#define PCI_DEVICE_HEADER_TYPE_PCI_TO_CARDBUS 0x2

#define PCI_CONFIG_OFFSET_VENDOR_ID 0
#define PCI_CONFIG_OFFSET_SUBCLASS 0x9
#define PCI_CONFIG_OFFSET_CLASS_CODE 0xA
#define PCI_CONFIG_OFFSET_HEADER_TYPE 0xD

static uint32_t pci_config_read32(uint8_t _bus, uint8_t _device, uint8_t _func, uint8_t offset) {
  uint32_t address;
  uint32_t bus = (uint32_t) _bus;
  uint32_t device = (uint32_t) _device;
  uint32_t func = (uint32_t) _func;

  // create configuration address
  // Bit 31       Bits 30-24  Bits 23-16   Bits 15-11     Bits 10-8        Bits 7-0
  // Enable Bit   Reserved    Bus Number   Device Number  Function Number  Register Offset1
  address = (uint32_t) ((bus << 16) | (device << 11) |
      (func << 8) |
      (offset & ~0b11) | // the last two bit is always 0
      0x80000000u // enable bit
  );

  outl(PCI_CONFIG_ADDRESS, address);

  return inl(PCI_CONFIG_DATA);
}

static uint16_t pci_config_read16(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset) {
  uint32_t ret = pci_config_read32(bus, device, func, offset);
  return ret >> 8 * (offset & 0b11); // the returned value is aligned in dword
}

static uint8_t pci_config_read8(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset) {
  uint32_t ret = pci_config_read32(bus, device, func, offset);
  return ret >> 8 * (offset & 0b11); // the returned value is aligned in dword
}

static uint16_t pci_vendor_id(uint8_t bus, uint8_t device, uint16_t func) {
  return pci_config_read16(bus, device, func, PCI_CONFIG_OFFSET_VENDOR_ID);
}

static uint8_t pci_class_code(uint8_t bus, uint8_t device, uint16_t func) {
  return pci_config_read8(bus, device, func, PCI_CONFIG_OFFSET_CLASS_CODE);
}

// TODO
static void pci_init_device(uint8_t bus, uint8_t device, uint8_t function) {
}

static uint8_t pci_header_type(uint16_t bus, uint16_t slot, uint16_t func) {
  return pci_config_read8(bus, slot, func, PCI_CONFIG_OFFSET_HEADER_TYPE);
}

void pci_scan() {
  for (uint16_t bus = 0; bus < 256; bus++) {
    for (uint8_t device = 0; device < 32; device++) {
      int function = 0;

      uint16_t vendor_id = pci_vendor_id(bus, device, function);
      if (vendor_id == PCI_INVALID_VENDOR_ID) return; // device doesn't exist

      uint8_t headerType = pci_header_type(bus, device, function);

      // if the 7-th bit is set, the device has multiple functions
      int n_functions = headerType & 0x40 ? 8 : 1;
      for (function = 0; function < n_functions; function++) {
        if (pci_vendor_id(bus, device, function) != PCI_INVALID_VENDOR_ID) {
          pci_init_device(bus, device, function);
          uint8_t class_code = pci_class_code(bus, device, function);
          kprintf("PCI: vendor_id=0x%x, device=0x%x, function=0x%x, class=0x%x\n",
                  vendor_id,
                  device,
                  function,
                  class_code);
        }
      }
    }
  }
}

void init_pci() {
  pci_scan();
}
