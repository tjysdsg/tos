#ifndef TOS_TEST_KERNEL_TEST_H
#define TOS_TEST_KERNEL_TEST_H
// These header must be inside .h instead of .cpp, cuz we might have some macro overrides in them
#include "interrupt_test.h"
#include "malloc_test.h"
#include "pit_test.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __TOS_ENABLE_KERNEL_TESTS__
void run_kernel_tests();
#endif

#ifdef __cplusplus
}
#endif

#endif //TOS_TEST_KERNEL_TEST_H
