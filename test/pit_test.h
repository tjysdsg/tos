#ifndef TOS_TEST_PIT_TEST_H
#define TOS_TEST_PIT_TEST_H
#include "kernel/idt.h"
#include "kernel/pit.h"

inline void pit_test() {
  enable_interrupt();
  kassert(is_interrupt_enabled(), "PIT tests require interrupt to be enabled");

  uint64_t tick1 = get_pit_tick();
  for (int i = 0; i < 10000000; ++i) {
    asm volatile("nop");
  }

  uint64_t tick2 = get_pit_tick();

  for (int i = 0; i < 10000000; ++i) {
    asm volatile("nop");
  }

  uint64_t tick3 = get_pit_tick();

  kassert(tick2 > tick1, "get_pit_tick()");
  kassert(tick3 > tick2, "get_pit_tick()");

  kprintf("PIT tests passed\n");
}

#endif //TOS_TEST_PIT_TEST_H
