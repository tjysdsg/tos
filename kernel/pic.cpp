#include "pic.h"
#include "port.h"
#include "cpuid.h"
#include "kprintf.h"
#include "kpanic.h"

static uint32_t APIC_BASE_ADDR = 0xFEE00000;

static uint32_t init_x2apic() {
  // check V2 Extended Topology Enumeration Leaf, Intel IA manual Vol. 2A 3-232
  uint32_t eax = 0x1F, ebx = 0, ecx = 0, edx = 0;
  cpuid(&eax, &ebx, &ecx, &edx);

  // local x2APIC ID is stored in EBX[31:24] (Intel IA manual Vol. 2A 3-215)
  return edx;
}

static void enable_apic() {
  // Intel IA manual 2-4 Vol. 4
  uint64_t apic_base_msr = rdmsr(0x1B);
  APIC_BASE_ADDR = apic_base_msr & 0xFFFFF000;

  // developer should have already checked if APIC is enabled by using CPUID.1H
  kassert((apic_base_msr >> 11) & 0x1, "APIC should be present on chip");

  kprintf("Found and enabled APIC, APIC address is 0x%x\n", APIC_BASE_ADDR);
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

  if (has_apic)
    enable_apic();
  else if (has_x2apic) {
    lapic_id = init_x2apic();
    kprintf("x2apic supported, id = %d\n", lapic_id);
    // TODO: implement x2APIC
    kassert(false, "x2APIC is not implemented");
  } else {
    kassert(false, "Cannot enable APIC or x2APIC");
  }
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
