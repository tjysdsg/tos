#ifndef TOS_TEST_MALLOC_TEST_H
#define TOS_TEST_MALLOC_TEST_H

#include "kernel/kpanic.h"
#include "kernel/memory.h"
#include "kernel/kprintf.h"

void malloc_test() {
  uint32_t size = 100;
  uint8_t *a = (uint8_t *) malloc(size);
  kassert(a, "malloc() returns nullptr");

  kprintf("malloc() returns 0x%x\n", a);
  for (uint32_t i = 0; i < size; ++i) {
    a[i] = i * i;
  }

  free(a);
}

#endif //TOS_TEST_MALLOC_TEST_H
