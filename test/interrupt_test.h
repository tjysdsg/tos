#ifndef TOS_TEST_INTERRUPT_TEST_H
#define TOS_TEST_INTERRUPT_TEST_H

#include "kernel/kpanic.h"
#include "kernel/idt.h"

inline void interrupt_test() {
  kassert(is_interrupt_enabled(), "is_interrupt_enabled");

  disable_interrupt();
  kassert(!is_interrupt_enabled(), "disable_interrupt");

  {
    NO_INT_START();
    kassert(!is_interrupt_enabled(), "NO_INT_START when interrupt is originally disabled");
    NO_INT_END();
    kassert(!is_interrupt_enabled(), "NO_INT_END when interrupt is originally disabled");
  }

  enable_interrupt();
  kassert(is_interrupt_enabled(), "enable_interrupt");

  {
    NO_INT_START();
    kassert(!is_interrupt_enabled(), "NO_INT_START when interrupt is originally enabled");
    NO_INT_END();
    kassert(is_interrupt_enabled(), "NO_INT_END when interrupt is originally enabled");
  }

  // TODO: asm volatile("int whatever");

  kprintf("Interrupt tests passed\n");
}

#endif //TOS_TEST_INTERRUPT_TEST_H
