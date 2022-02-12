/// \file Programmable Interrupt Timer
#ifndef TOS_KERNEL_PIT_H
#define TOS_KERNEL_PIT_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void init_pit_timer(uint32_t frequency);

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_PIT_H
