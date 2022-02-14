#include "kpanic.h"
#include "kprintf.h"

void _kpanic(const char *err, const char *file, int line, const char *func) {
  kprintf("KERNEL PANIC at %s:%d %s(): %s\n", file, line, func, err);
  asm volatile("hlt");
}

void _assert_fail(const char *err, const char *file, int line, const char *func) {
  kprintf("Assertion failed at %s:%d %s(): %s\n", file, line, func, err);
  asm volatile("hlt");
}
