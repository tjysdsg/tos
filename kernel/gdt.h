#ifndef TOS_KERNEL_GDT_H
#define TOS_KERNEL_GDT_H

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t gdt_initialized;
extern uint16_t gdt_code_seg_selector;
extern uint16_t gdt_data_seg_selector;

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_GDT_H
