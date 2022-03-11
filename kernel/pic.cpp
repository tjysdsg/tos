#include "pic.h"
#include "port.h"
#include "cpuid.h"
#include "kprintf.h"
#include "kpanic.h"
#include "isr.h"

static uint32_t APIC_BASE_ADDR = 0xFEE00000;
static uint32_t IOAPIC_BASE_ADDR = 0xFEC00000;

#define IOAPIC_REG_VERSION 0x1
#define IOAPIC_REG_REDTABLE 0x10

typedef struct {
  uint32_t io_reg_sel;
  uint32_t pad[3];
  uint32_t io_win;
} ioapic_t;
static volatile ioapic_t *ioapic = nullptr;

uint32_t read_apic_register(uint32_t offset) {
  return *(uint32_t *) (APIC_BASE_ADDR + offset);
}

void write_apic_register(uint32_t offset, uint32_t val) {
  *(uint32_t *) (APIC_BASE_ADDR + offset) = val;
}

static uint32_t init_x2apic() {
  // check V2 Extended Topology Enumeration Leaf, Intel IA manual Vol. 2A 3-232
  uint32_t eax = 0x1F, ebx = 0, ecx = 0, edx = 0;
  cpuid(&eax, &ebx, &ecx, &edx);

  // local x2APIC ID is stored in EBX[31:24] (Intel IA manual Vol. 2A 3-215)
  return edx;
}

static uint32_t read_ioapic_register(uint32_t reg) {
  ioapic->io_reg_sel = reg;
  return ioapic->io_win;
}

static void write_ioapic_register(uint32_t reg, uint32_t data) {
  ioapic->io_reg_sel = reg;
  ioapic->io_win = data;
}

static void init_io_apic() {
  // TODO: get IOAPIC_BASE_ADDR from ACPI
  ioapic = (volatile ioapic_t *) IOAPIC_BASE_ADDR;

  uint32_t max_redirection_entry = (read_ioapic_register(IOAPIC_REG_VERSION) >> 16) & 0xFF;

  // mark all interrupts edge-triggered, active high, disabled, and not routed to any CPUs
  for (uint32_t i = 0; i <= max_redirection_entry; i++) {
    write_ioapic_register(IOAPIC_REG_REDTABLE + 2 * i, 0x10000 | (IRQ0 + i));
    write_ioapic_register(IOAPIC_REG_REDTABLE + 2 * i + 1, 0);
  }
}

static void enable_apic() {
  // Intel IA manual 2-4 Vol. 4
  uint64_t apic_base_msr = rdmsr(0x1B);
  APIC_BASE_ADDR = apic_base_msr & 0xFFFFF000;

  // developer should have already checked if APIC is enabled by using CPUID.1H
  kassert((apic_base_msr >> 11) & 0x1, "APIC should be present on chip");

  // software-enable APIC using the spurious vector register, 10-34 Vol. 3A
  // 0xFF used according to https://wiki.osdev.org/APIC
  write_apic_register(APIC_REG_SVR, 0xFF | 0x100);

  kprintf("Found and enabled APIC, APIC address is 0x%x\n", APIC_BASE_ADDR);

  // mask LINT0 and LINT1
  // typically in MP systems, the LINT0 and LINT1 pins are not used to deliver interrupts to the logical processors
  // 8-32 Vol. 3A
  write_apic_register(APIC_REG_LINT0, 0x10000);
  write_apic_register(APIC_REG_LINT1, 0x10000);

  // clear error status
  write_apic_register(APIC_REG_ESR, 0);
  // clear unfinished interrupt
  write_apic_register(APIC_REG_EOI, 0);
  // accept all level of interrupts
  write_apic_register(APIC_REG_TPR, 0);
}

void init_apic() {
  disable_pic();

  uint32_t lapic_id = 0;
  bool has_x2apic = false;
  bool has_apic = false;

  { // get initial local APIC ID, and check if x2APIC is supported
    uint32_t eax = 0x01, ebx = 0, ecx = 0, edx = 0;
    cpuid(&eax, &ebx, &ecx, &edx);

    // local APIC ID is stored in EBX[31:24] (Intel IA manual Vol. 2A 3-215)
    lapic_id = (ebx >> 24) & 0xFF;
    kprintf("Initial lapic_id = %d\n", lapic_id);

    // 3-238 Vol. 2A
    has_x2apic = (ecx >> 21) & 0x1;

    // 3-240 Vol. 2A
    has_apic = (edx >> 9) & 0x1;
  }

  if (has_apic) {
    enable_apic();
    kassert(read_apic_register(0x20) == lapic_id, "Inconsistency between the local APIC ID from CPUID and MSR");
  } else if (has_x2apic) {
    lapic_id = init_x2apic();
    kprintf("x2apic supported, id = %d\n", lapic_id);
    // TODO: implement x2APIC
    kassert(false, "x2APIC is not implemented");
  } else {
    kassert(false, "Cannot enable APIC or x2APIC");
  }

  init_io_apic();
}

void remap_pic() {
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0);
  outb(0xA1, 0x0);
}

void disable_pic() {
  outb(0x21, 0xFF);
  outb(0xA1, 0xFF);
}

void send_eoi() {
  write_apic_register(APIC_REG_EOI, 0);
}

// doc/ioapic.pdf page 11-12
void enable_ioapic_irq(uint32_t from, uint32_t to, uint32_t lapic_id) {
  kassert(from < 24, "IOAPIC supports at most 24 redirection entries");

  // mark interrupt edge-triggered, active high, enabled, and routed to the given cpu
  write_ioapic_register(IOAPIC_REG_REDTABLE + 2 * from, to);
  write_ioapic_register(IOAPIC_REG_REDTABLE + 2 * from + 1, lapic_id << 24);
}

static uint32_t apic_tick = 0;

static void apic_timer_callback(registers_t *regs) {
  apic_tick++;
  // kprintf("APIC timer Tick: %d\n", apic_tick);
}

static void reset_apic_timer(uint32_t reset_value) {
  write_apic_register(APIC_REG_TICR, reset_value);
}

static void stop_apic_timer() {
  // Intel IA manual 10-16 Vol. 3A
  write_apic_register(APIC_REG_TICR, 0);
}

// https://wiki.osdev.org/APIC_timer
void init_apic_timer() {
  register_interrupt_handler(APIC_TIMER, &apic_timer_callback);

  // setup timer, Intel IA manual 10-16 Vol. 3A
  write_apic_register(APIC_REG_TDCR, 0xB); // divide timer counts by 1
  write_apic_register(APIC_REG_TIMER, 0x20000 | APIC_TIMER); // periodic, bind to corresponding IRQ
  reset_apic_timer(0xFFFFFFFF);
}
