#include "pit.h"
#include "isr.h"
#include "kprintf.h"
#include "pic.h"

static uint32_t tick = 0;

static void timer_callback(registers_t *regs) {
  tick++;
  // kprintf("Tick: %d\n", tick);
}

// https://wiki.osdev.org/Programmable_Interval_Timer
void init_pit_timer(uint32_t frequency) {
  register_interrupt_handler(IRQ0, &timer_callback);

  // setup timer, Intel IA manual 10-16 Vol. 3A
  // TODO: find the clock frequency using CPUID leaf 0x15, and adjust timer divisor accordingly
  write_apic_register(APIC_REG_TDCR, 0xB); // divide timer counts by 1
  write_apic_register(APIC_REG_TIMER, 0x20000 | IRQ0); // periodic, bind to IRQ0
  write_apic_register(APIC_REG_TICR, 10000000);
}
