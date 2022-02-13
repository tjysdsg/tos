#ifndef TOS_KERNEL_ISR_H
#define TOS_KERNEL_ISR_H
#include <stdint.h>

// must be the same as in idt.asm
#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

#define PIC1 0x20 // address for master PIC
#define PIC2 0xA0 // address for slave PIC
#define PIC1_COMMAND    PIC1
#define PIC1_DATA    (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA    (PIC2+1)

#define PIC_EOI 0x20 // End-of-interrupt command code

#ifdef __cplusplus
extern "C" {
#endif

void init_interrupt_handlers();

void isr0();
void isr1();
void isr2();
void isr3();
void isr4();
void isr5();
void isr6();
void isr7();
void isr8();
void isr9();
void isr10();
void isr11();
void isr12();
void isr13();
void isr14();
void isr15();
void isr16();
void isr17();
void isr18();
void isr19();
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

void irq0();
void irq1();
void irq2();
void irq3();
void irq4();
void irq5();
void irq6();
void irq7();
void irq8();
void irq9();
void irq10();
void irq11();
void irq12();
void irq13();
void irq14();
void irq15();

struct registers_t {
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by pushad
  uint32_t int_no, err_code;                       // interrupt number and error code (if applicable)
  uint32_t eip, cs, eflags;                        // pushed by the processor automatically
};

void isr_handler(registers_t regs);
void irq_handler(registers_t regs);

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(uint8_t n, isr_t handler);

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_ISR_H
