#ifndef TOS_TEST_PIT_TEST_H
#define TOS_TEST_PIT_TEST_H
#include "kernel/idt.h"
#include "kernel/pit.h"
#include "kernel/nop.h"

#define KERNEL_PIT_FREQUENCY 200

inline void pit_test() {
  init_pit(KERNEL_PIT_FREQUENCY);

  enable_interrupt();
  kassert(is_interrupt_enabled(), "PIT tests require interrupt to be enabled");

  { // test ticking
    uint32_t tick1 = get_pit_tick();
    for (int i = 0; i < 1000; ++i) {
      kprintf("Testing PIT...\r");
    }
    kprintf("\n");
    uint32_t tick2 = get_pit_tick();
    kassert(tick2 > tick1, "get_pit_tick()");
  }

  { // test sleep
    uint32_t tick1 = get_pit_tick();
    pit_sleep(150);
    uint32_t tick2 = get_pit_tick();
    kassert(tick2 == tick1 + 30, "pit_sleep");
  }

  kprintf("PIT tests passed\n");
}

#endif //TOS_TEST_PIT_TEST_H
