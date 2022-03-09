/// https://itanium-cxx-abi.github.io/cxx-abi/abi.html#dso-dtor-runtime-api

#include "cxxabi.h"

struct atexit_func_entry_t {
  void (*destructor_func)(void *);
  void *obj_ptr;
  void *dso_handle;
} __atexit_funcs[ATEXIT_MAX_FUNCS];

unsigned __atexit_func_count = 0;

void *__dso_handle = __atexit_funcs; // FIXME: how to initialize a unique dso handle?

int __cxa_atexit(void (*f)(void *), void *objptr, void *dso) {
  // TODO: dynamically expand __atexit_funcs[] when full
  if (__atexit_func_count >= ATEXIT_MAX_FUNCS) { return -1; }
  __atexit_funcs[__atexit_func_count].destructor_func = f;
  __atexit_funcs[__atexit_func_count].obj_ptr = objptr;
  __atexit_funcs[__atexit_func_count].dso_handle = dso;
  __atexit_func_count++;
  return 0;
};

void __cxa_finalize(void *d) {
  unsigned i = __atexit_func_count;
  if (!d) { /// if d is nullptr, call all destructors
    while (i--) {
      if (__atexit_funcs[i].destructor_func)
        (*__atexit_funcs[i].destructor_func)(__atexit_funcs[i].obj_ptr);
    }
    return;
  }

  while (i--) {
    // TODO: remove finished entries to save space
    if (__atexit_funcs[i].dso_handle == d) {
      (*__atexit_funcs[i].destructor_func)(__atexit_funcs[i].obj_ptr);
      __atexit_funcs[i].destructor_func = nullptr;
      __atexit_funcs[i].dso_handle = nullptr;
      __atexit_funcs[i].obj_ptr = nullptr;
    }
  }
}

/* TODO: use a lock to implement guards, and remove the -fno-threadsafe-statics compling flag
extern "C" int __cxa_guard_acquire(__guard *g) {
  return !*(char *) (g);
}

extern "C" void __cxa_guard_release(__guard *g) {
  *(char *) g = 1;
}

extern "C" void __cxa_guard_abort(__guard *) {
}
*/
