#ifndef TOS_KERNEL_IDT_H
#define TOS_KERNEL_IDT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// https://wiki.osdev.org/Interrupt_Descriptor_Table
struct idt_entry_t {
  uint16_t offset_low;        // offset bits 0..15
  uint16_t selector;          // a code segment selector in GDT or LDT
  uint8_t reserved;           // unused, set to 0
  uint8_t flags;              /* - task/trap/interrupt, x16/x32
                                 - 0 bit
                                 - privilege
                                 - present bit
                                 - offset bits 16..31
                              */
  uint16_t offset_high;       // offset bits 16..31
} __attribute__((packed));

struct idtr_t {
  uint16_t size;
  uint32_t offset;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));

void init_idt();
void enable_interrupt();

extern void flush_idt(uint32_t idtr);

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_IDT_H
