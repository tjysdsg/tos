#include "isr.h"
#include "kprintf.h"
#include "pic.h"

#define N_INT_HANDLERS 256
isr_t interrupt_handlers[N_INT_HANDLERS]{};

void init_interrupt_handlers() {
  for (auto &interrupt_handler: interrupt_handlers)
    interrupt_handler = nullptr;
}

void isr_handler(registers_t regs) {
  isr_t handler = interrupt_handlers[regs.int_no];
  kprintf("Receiving INT %d, error code: 0x%x handler at: 0x%x\n", regs.int_no, regs.err_code, handler);
  if (handler)
    handler(&regs);
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
  interrupt_handlers[n] = handler;
}

void irq_handler(registers_t regs) {
  send_eoi();

  isr_t handler = interrupt_handlers[regs.int_no];

  if (regs.int_no - IRQ0 != 0)
    kprintf("Receiving IRQ %d, handler at: 0x%x\n", regs.int_no - IRQ0, handler);

  if (handler) {
    handler(&regs);
  }
}
