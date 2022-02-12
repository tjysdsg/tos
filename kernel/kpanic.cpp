#include "kpanic.h"
#include "kprintf.h"

void kpanic(const char *err) {
  kprintf("KERNEL PANIC:\n");
  kprintf(err);
  kprintf("\n");

  asm volatile("hlt");
}
