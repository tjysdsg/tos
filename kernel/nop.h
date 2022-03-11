#ifndef TOS_KERNEL_NOP_H
#define TOS_KERNEL_NOP_H

/**
 * @brief No op, compiler is prevented from optimizing this
 */
inline void nop() {
  asm volatile("nop");
}

#endif //TOS_KERNEL_NOP_H
