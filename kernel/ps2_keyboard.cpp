#include "ps2_keyboard.h"
#include "isr.h"
#include "tty.h"
#include "port.h"
#include "pic.h"
#include "kprintf.h"

#define NO -1
char PS2_NORMAL_KEYS[256] = {
    NO, /* esc */ 0x1B, '1', '2', '3', '4', '5', '6',  // 0x00
    '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',  // 0x10
    'o', 'p', '[', ']', '\n', NO, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',  // 0x20
    '\'', '`', NO, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', NO, '*',  // 0x30
    NO, ' ', NO, NO, NO, NO, NO, NO,
    NO, NO, NO, NO, NO, NO, NO, '7',  // 0x40
    '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', NO, NO, NO, NO,   // 0x50
};

static void keyboard_interrupt_handler(registers_t *regs) {
  uint8_t key = inb(0x60);

  char c = -1;
  if (key <= 0xD3)
    c = PS2_NORMAL_KEYS[key];

  // TODO: recognize other keys

  if (c >= 0)
    tty_show_keyboard_input(c);
}

void init_ps2_keyboard() {
  register_interrupt_handler(APIC_KBD, keyboard_interrupt_handler);
  enable_ioapic_irq(IOAPIC_KBD, APIC_KBD, 0); // let IO APIC pass keyboard interrupts to BSP
}
