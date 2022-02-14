#include "kernel_test.h"
#include "malloc_test.h"

#ifdef __TOS_ENABLE_KERNEL_TESTS__

void run_kernel_tests() {
  malloc_test();
}

#endif
