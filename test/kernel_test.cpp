#include "kernel_test.h"
#include "interrupt_test.h"
#include "malloc_test.h"
#include "pit_test.h"

#ifdef __TOS_ENABLE_KERNEL_TESTS__

void run_kernel_tests() {
  interrupt_test();
  pit_test();
  malloc_test();
}

#endif
