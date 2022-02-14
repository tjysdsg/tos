#include "pit.h"
#include "isr.h"
#include "kprintf.h"
#include "port.h"

static uint32_t tick = 0;

static void timer_callback(registers_t *regs) {
  tick++;
  // kprintf("Tick: %d\n", tick);
}

// https://wiki.osdev.org/Programmable_Interval_Timer
void init_pit_timer(uint32_t frequency) {
  register_interrupt_handler(IRQ0, &timer_callback);

  // the value we send to the PIT is the divisor of the input clock (1193180 Hz)
  uint16_t divisor = 1193180 / frequency;

  // 0x43: timer mode/command port
  // 0x36:
  // - channel 0
  // - access mode: lobyte/hibyte
  // - square wave generator
  // - binary mode
  outb(0x43, 0x36);

  // divisor has to be sent byte-wise, so split here into upper/lower bytes
  auto l = (uint8_t) (divisor & 0xFF);
  auto h = (uint8_t) ((divisor >> 8) & 0xFF);

  // send the frequency divisor
  outb(0x40, l);
  outb(0x40, h);
}
