/// \file Programmable Interrupt Timer
#ifndef TOS_KERNEL_PIT_H
#define TOS_KERNEL_PIT_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Init Programmable Interval Timer
 * @param frequency
 */
void init_pit(uint32_t frequency);

/**
 * @brief Get the PIT's frequency, same as the frequency passed to init_pit
 */
uint32_t get_pit_freq();

uint32_t get_pit_tick();

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_PIT_H
