#ifndef TOS_KERNEL_KPANIC_H
#define TOS_KERNEL_KPANIC_H

#ifdef __cplusplus
extern "C" {
#endif

#define kassert(x, msg) do { \
  if (!(x)) {                \
    kpanic((msg));           \
  }                          \
} while(0)

void kpanic(const char *err);

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_KPANIC_H
