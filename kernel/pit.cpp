#include "pit.h"
#include "isr.h"
#include "idt.h"
#include "kprintf.h"
#include "port.h"
#include "pic.h"
#include "nop.h"

static uint32_t pit_tick = 0;
static uint32_t pit_freq = 1193180;

static void pit_timer_callback(registers_t *regs) {
  ++pit_tick;
  // kprintf("PIT tick: %d\n", pit_tick);
}

// https://wiki.osdev.org/Programmable_Interval_Timer
void init_pit(uint32_t frequency) {
  NO_INT_START();

  register_interrupt_handler(APIC_PIT, &pit_timer_callback);
  enable_ioapic_irq(IOAPIC_PIT, APIC_PIT, 0); // let IO APIC pass PIT interrupts to BSP

  // the value we send to the PIT is the divisor of the input clock (1193180 Hz)
  uint16_t divisor = 1193180 / (uint16_t) frequency;

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

  pit_freq = frequency;

  NO_INT_END();
}

uint32_t get_pit_freq() { return pit_freq; }

uint32_t get_pit_tick() { return pit_tick; }
