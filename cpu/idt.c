#include "idt.h"
#include "../kernel/util.h"

void set_idt_gate(int n, uint32_t handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; /* 1000: present=1,priviledge=ring0,storage
                            segment=interrupt and trap gate;
                            1110: interrupt gate */
    idt[n].high_offset = high_16(handler);
}

void set_idt() {
    idt_reg.base = (uint32_t)&idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    /* load idt register*/
    __asm__ __volatile__("lidtl (%0)" : : "r"(&idt_reg));
}
