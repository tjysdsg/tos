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

struct registers_t {
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by pushad
  uint32_t int_no, err_code;                       // interrupt number and error code (if applicable)
  uint32_t eip, cs, eflags;                        // pushed by the processor automatically
};

void isr_handler(registers_t regs);

}

#endif //TOS_KERNEL_ISR_H
