#ifndef TOS_KERNEL_CXXABI_H
#define TOS_KERNEL_CXXABI_H

#define ATEXIT_MAX_FUNCS    128

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned uarch_t;

struct atexit_func_entry_t {
  // Each member is at least 4 bytes large. Such that each entry is 12 bytes. 128 * 12 = 1.5KB exact.
  void (*destructor_func)(void *);
  void *obj_ptr;
  void *dso_handle;
};

int __cxa_atexit(void (*f)(void *), void *objptr, void *dso);
void __cxa_finalize(void *f);

// guard variables

// The ABI requires a 64-bit type
__extension__ typedef int __guard __attribute__((mode(__DI__)));
extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release(__guard *);
extern "C" void __cxa_guard_abort(__guard *);

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_CXXABI_H
