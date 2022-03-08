#ifndef TOS_KERNEL_PIC_H
#define TOS_KERNEL_PIC_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Init advanced programmable interrupt controller if supported, this will disable the 8259 pic
 * @details Prefer x2APIC over APIC
 */
void init_apic();

/**
 * @brief Remap 8259 PIC's IRQ 0-15 to 32-47, since 0-31 is reserved for exceptions/faults
 */
void remap_pic();

/**
 * @brief Disable 8259 PIC
 */
void disable_pic();

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_PIC_H
