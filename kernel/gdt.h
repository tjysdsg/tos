#ifndef TOS_KERNEL_GDT_H
#define TOS_KERNEL_GDT_H

extern "C" uint8_t gdt_initialized;
extern "C" uint16_t gdt_code_seg_selector;
extern "C" uint16_t gdt_data_seg_selector;

#endif //TOS_KERNEL_GDT_H
