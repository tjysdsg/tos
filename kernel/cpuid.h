#ifndef TOS_KERNEL_CPUID_H
#define TOS_KERNEL_CPUID_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void cpuid(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);

uint64_t rdmsr(uint32_t msr);
void wrmsr(uint32_t msr, uint64_t val);

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_CPUID_H
