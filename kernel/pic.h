#ifndef TOS_KERNEL_PIC_H
#define TOS_KERNEL_PIC_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Intel IA manual Vol. 3A 10-7
#define APIC_REG_SVR 0xF0 // spurious vector register
#define APIC_REG_TPR 0x80 // task priority register
#define APIC_REG_EOI 0xB0 // end of interrupt
#define APIC_REG_ESR 0x280 // error status register
#define APIC_REG_LINT0 0x0350
#define APIC_REG_LINT1 0x0360
#define APIC_REG_TDCR 0x03E0 // timer divide configuration
#define APIC_REG_TIMER 0x0320 // LVT timer register
#define APIC_REG_TICR 0x0380 // timer initial count register
#define APIC_REG_TCCR 0x0390 // timer current count register

#define IOAPIC_KBD 1
#define IOAPIC_PIT 2 /* PIT is mapped to IOAPIC via IRQ2 instead of IRQ0
                      https://wiki.osdev.org/HPET#.22Legacy_replacement.22_mapping */

#define APIC_TIMER (IRQ0 + 0)
#define APIC_KBD (IRQ0 + 1)
#define APIC_PIT (IRQ0 + 15)

/**
 * @brief Init advanced programmable interrupt controller if supported, this will disable the 8259 pic
 * @details Prefer x2APIC over APIC
 */
void init_apic();

/**
 * @brief Init APIC timer, and calibrate its frequency
 */
void init_apic_timer();

/**
 * @brief Remap 8259 PIC's IRQ 0-15 to 32-47, since 0-31 is reserved for exceptions/faults
 */
void remap_pic();

/**
 * @brief Disable 8259 PIC
 */
void disable_pic();

void send_eoi();

void write_apic_register(uint32_t offset, uint32_t val);
uint32_t read_apic_register(uint32_t offset);

/**
 * @brief Make sure to call this to bind an IRQ triggered by IO APIC to a CPU's local APIC
 * @param from The original interrupt number IOAPIC got (e.g. 1)
 * @param to The destination interrupt number passed to CPU's local APIC (e.g. IRQ1)
 */
void enable_ioapic_irq(uint32_t from, uint32_t to, uint32_t lapic_id);

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_PIC_H
