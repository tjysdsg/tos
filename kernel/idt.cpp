#include "idt.h"
#include "isr.h"
#include "gdt.h"
#include "kprintf.h"
#include <stdint.h>

#define IDT_TASK_GATE           0x5
#define IDT_32BIT_INT_GATE      0xE
#define IDT_32BIT_TRAP_GATE     0xF

#define IDT_PRESENT_BIT         0b10000000
#define IDT_RING_0              0b00000000
#define DEFAULT_IDT_FLAG        (IDT_PRESENT_BIT | IDT_RING_0 | IDT_32BIT_INT_GATE)

idt_entry_t idt_entries[256]{};
idtr_t idtr{};

void create_idt_entry(idt_entry_t *entry, uint32_t offset, uint16_t selector, uint8_t flags) {
  entry->offset_low = offset & 0x0000FFFF;
  entry->offset_high = offset >> 16;
  entry->selector = selector;
  entry->reserved = 0;
  entry->flags = flags;
}

void init_idt() {
  static_assert(sizeof(idt_entry_t) == 8);

  create_idt_entry(&idt_entries[0], (uint32_t) isr0, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[1], (uint32_t) isr1, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[2], (uint32_t) isr2, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[3], (uint32_t) isr3, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[4], (uint32_t) isr4, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[5], (uint32_t) isr5, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[6], (uint32_t) isr6, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[7], (uint32_t) isr7, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[8], (uint32_t) isr8, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[9], (uint32_t) isr9, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[10], (uint32_t) isr10, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[11], (uint32_t) isr11, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[12], (uint32_t) isr12, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[13], (uint32_t) isr13, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[14], (uint32_t) isr14, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[15], (uint32_t) isr15, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[16], (uint32_t) isr16, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[17], (uint32_t) isr17, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[18], (uint32_t) isr18, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[19], (uint32_t) isr19, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[20], (uint32_t) isr20, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[21], (uint32_t) isr21, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[22], (uint32_t) isr22, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[23], (uint32_t) isr23, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[24], (uint32_t) isr24, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[25], (uint32_t) isr25, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[26], (uint32_t) isr26, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[27], (uint32_t) isr27, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[28], (uint32_t) isr28, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[29], (uint32_t) isr29, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[30], (uint32_t) isr30, gdt_code_seg_selector, DEFAULT_IDT_FLAG);
  create_idt_entry(&idt_entries[31], (uint32_t) isr31, gdt_code_seg_selector, DEFAULT_IDT_FLAG);

  idtr.size = sizeof(idt_entry_t) * 256 - 1;
  idtr.offset = (uint32_t) idt_entries;
  flush_idt((uint32_t) &idtr);
}