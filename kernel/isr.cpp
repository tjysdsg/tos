#include "isr.h"
#include "kprintf.h"

void isr_handler(registers_t regs) {
  kprintf("Recieved interrupt: %d\n", regs.int_no);
  kprintf("Errno: %d\n", regs.err_code);
}
