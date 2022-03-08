#include "cpuid.h"

void cpuid(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) { //NOLINT(readability-non-const-parameter)
  asm volatile("cpuid": "=a" (*a), "=b" (*b), "=c" (*c), "=d" (*d) : "a" (*a), "b" (*b), "c" (*c), "d" (*d));
}

uint64_t rdmsr(uint32_t msr) {
  uint32_t edx = 0, eax = 0;
  asm volatile("rdmsr": "=d" (edx), "=a" (eax) : "c" (msr));

  uint64_t ret = edx;
  return (ret << 32) + eax;
}

void wrmsr(uint32_t msr, uint64_t val) {
  uint32_t eax = val & 0xFFFFFFFF;
  uint32_t edx = (val >> 32) & 0xFFFFFFFF;
  asm volatile("wrmsr"::"d" (edx), "a" (eax), "c" (msr));
}
