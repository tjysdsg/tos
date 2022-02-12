#ifndef TOS_KERNEL_ISR_H
#define TOS_KERNEL_ISR_H
#include <stdint.h>

extern "C" {

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

}

#endif //TOS_KERNEL_ISR_H
