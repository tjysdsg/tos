#ifndef TOS_KERNEL_KPANIC_H
#define TOS_KERNEL_KPANIC_H
#include "kprintf.h"

#ifdef __cplusplus
extern "C" {
#endif

#define kassert(x, msg) do {                          \
  if (!(x)) {                                         \
    _assert_fail(msg,  __FILE__, __LINE__, __func__); \
  }                                                   \
} while(0)

#define kpanic(msg) do {                         \
    _kpanic(msg,  __FILE__, __LINE__, __func__); \
} while(0)

void _kpanic(const char *err, const char *file, int line, const char *func);
void _assert_fail(const char *err, const char *file, int line, const char *func);

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_KPANIC_H
