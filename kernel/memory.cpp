#include <stdint.h>
#include "kpanic.h"
#include "memory.h"
#include "kprintf.h"

extern uint8_t _kernel_seg_end; /// see linker.ld
static uint32_t kernel_mem_start = 0;
static uint32_t kernel_free_mem = 0; /// current starting address of free memory

void init_kmalloc() { // find kernel memory start address, align by 4KB
  kernel_mem_start = (uint32_t) &(_kernel_seg_end);
  kernel_mem_start += 0x1000; // reserve some space to be safe

  if (kernel_mem_start & 0x00000FFF) {
    kernel_mem_start &= 0xFFFFF000;
    kernel_mem_start += 0x1000;
  } else {
    kernel_mem_start &= 0xFFFFF000;
  }

  kprintf("Kernel memory start = 0x%x\n", kernel_mem_start);
  kernel_free_mem = kernel_mem_start;
}

uint32_t kmalloc_page_align(uint32_t size) {
  // page-align the address if not
  if (size & 0x00000FFF) {
    size &= 0xFFFFF000;
    size += 0x1000;
  } else {
    size &= 0xFFFFF000;
  }

  uint32_t ret = kernel_free_mem;
  kernel_free_mem += size;
  // kprintf("kmalloc size: 0x%x\n", size);
  // kprintf("kmalloc ret: 0x%x\n", ret);
  // kprintf("kmalloc new kernel_free_mem: 0x%x\n", kernel_free_mem);
  return ret;
}

void *kmemset(void *ptr, int c, uint32_t n) {
  auto *p = (uint8_t *) ptr;
  for (uint32_t i = 0; i < n; ++i) {
    *(p++) = c;
  }
  return ptr;
}
