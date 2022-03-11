#include "kernel_test.h"

#ifdef __TOS_ENABLE_KERNEL_TESTS__

void run_kernel_tests() {
  interrupt_test();
  pit_test();
  malloc_test();
}

#endif
