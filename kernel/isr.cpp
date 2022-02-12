#include "isr.h"
#include "kprintf.h"
#include "port.h"

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

isr_t interrupt_handlers[256]{};

void isr_handler(registers_t regs) {
  kprintf("Recieved interrupt: %d\n", regs.int_no);
  kprintf("Errno: %d\n", regs.err_code);
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
  interrupt_handlers[n] = handler;
}

void irq_handler(registers_t regs) {
  // send End-Of-Interrupt signal to PIC if the interrupt involves the slave
  if (regs.int_no >= IRQ8) {
    // reset slave
    outb(0xA0, 0x20);
  }
  // reset master no matter where is signal is from
  outb(0x20, 0x20);

  kprintf("Receiving IRQ %d\n", regs.int_no - 32);

  isr_t handler = interrupt_handlers[regs.int_no];
  if (handler) {
    handler(regs);
  }
}
