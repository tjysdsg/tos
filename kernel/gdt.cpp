#include "gdt.h"
#include "kprintf.h"
#include <stdint.h>

GDT gdt;
extern "C" GDTR gdtr;

/**
 * \param target A pointer to the 8-byte GDT entry
 * \param source An arbitrary structure describing the GDT entry
 */
void encode_gdt_entry(uint8_t *target, gdt_entry source) {
  // Check the limit to make sure that it can be encoded
  if ((source.limit > 65536) && ((source.limit & 0xFFF) != 0xFFF)) {
    kprintf("You can't do that!");
  }
  if (source.limit > 65536) {
    // Adjust granularity if required
    source.limit = source.limit >> 12;
    target[6] = 0xC0;
  } else {
    target[6] = 0x40;
  }

  // Encode the limit
  target[0] = source.limit & 0xFF;
  target[1] = (source.limit >> 8) & 0xFF;
  target[6] |= (source.limit >> 16) & 0xF;

  // Encode the base
  target[2] = source.base & 0xFF;
  target[3] = (source.base >> 8) & 0xFF;
  target[4] = (source.base >> 16) & 0xFF;
  target[7] = (source.base >> 24) & 0xFF;

  // And... Type
  target[5] = source.type;
}

extern "C" void set_gdt();
void init_gdt() {
  encode_gdt_entry((uint8_t *) &gdt.null, {.base=0, .limit=0, .type=0});
  // encode_gdt_entry((uint8_t *) &gdt.code, {.base=0x400000, .limit=0xFFFFF, .type=0x9A});
  // encode_gdt_entry((uint8_t *) &gdt.data, {.base=0x800000, .limit=0xFFFFF, .type=0x92});
  *((uint64_t *) &gdt.code) = 0x4CF96000000FFFF;
  *((uint64_t *) &gdt.data) = 0x8CF9C000000FFFF;

  gdtr.offset = (uint32_t) &gdt;
  gdtr.size = sizeof(gdt) - 1;
  set_gdt();

  kprintf("gdt addr: %x\n", &gdt);
  kprintf("gdtr offset: %x\n", gdtr.offset);

  auto *pgdt = (uint64_t *) &gdt;
  kprintf("gdt[0]: %x\n", pgdt[0]);
  kprintf("gdt[1]: %x\n", pgdt[1]);
  kprintf("gdt[2]: %x\n", pgdt[2]);
}
