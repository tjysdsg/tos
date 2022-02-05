#ifndef TOS_KERNEL_GDT_H_
#define TOS_KERNEL_GDT_H_
#include <stdint.h>

/// human friendly gdt entry
struct gdt_entry {
  uint32_t base = 0;
  uint32_t limit = 0;
  uint16_t type = 0;
} __attribute__((packed));

struct GDT {
  gdt_entry null; // Selector 0x00 cannot be used
  gdt_entry code; // Selector 0x08 will be our code
  gdt_entry data; // Selector 0x10 will be our data
  // TODO: TSS
} __attribute__((packed)) __attribute__((aligned(0x1000)));

struct GDTR {
  uint16_t size;
  uint32_t offset;
} __attribute__((packed));

void init_gdt();

/// https://wiki.osdev.org/Global_Descriptor_Table
/// real memory layout
// struct gdt_entry {
//   uint16_t limit_low;
//   uint16_t base_low;
//   uint8_t base_middle;
//
//   /// below is the bits of the 8-bit access field
//   uint8_t access_Ac: 1;
//   uint8_t access_RW: 1;
//   uint8_t access_DC: 1;
//   uint8_t access_Ex: 1;
//   uint8_t access_S: 1;
//   uint8_t access_Privl: 2;
//   uint8_t access_Pr: 1;
//
//   uint8_t limit_high: 4;
//
//   /// below is the bits of 4-bit flags field
//   uint8_t flags_reserved: 1;
//   uint8_t flags_L: 1;
//   uint8_t flags_Sz: 1;
//   uint8_t flags_Gr: 1;
//
//   uint8_t base_high;
// } __attribute__((packed));

#endif //TOS_KERNEL_GDT_H_
