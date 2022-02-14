#include <stdint.h>
#include "kpanic.h"
#include "memory.h"
#include "kprintf.h"
#include "isr.h"

#define MEMORY_HEADER_SIZE sizeof(memory_block_header_t)

/// split a free memory block into two if there is at least some extra space with this size
#define MIN_MEMORY_SPLIT_THRESHOLD 4

extern uint8_t _kernel_seg_end; /// see linker.ld
static uint32_t kernel_free_mem = 0; /// current starting address of free memory
static heap_t *heap;

/// https://wiki.osdev.org/Exceptions#General_Protection_Fault
static void general_protection_fault_handler(registers_t regs) {
  kprintf("General protection fault, segment selector index = 0x%x\n", regs.err_code);
  kpanic("See above");
}

static uint32_t align_addr_by_4k(uint32_t addr) {
  if (addr & 0x00000FFF) {
    addr &= 0xFFFFF000;
    addr += 0x1000;
  } else {
    addr &= 0xFFFFF000;
  }
  return addr;
}

// must be called before any memory-related operation is performed
void init_memory() {
  kernel_free_mem = (uint32_t) &(_kernel_seg_end);
  kernel_free_mem += 0x1000; // reserve a 4KB empty space to be safe

  // find kernel free memory starting location, align by 4KB
  kernel_free_mem = align_addr_by_4k(kernel_free_mem);
  kprintf("Kernel free memory starts at: 0x%x\n", kernel_free_mem);

  // register memory-related interrupt handler
  register_interrupt_handler(13, general_protection_fault_handler);
}

void init_heap() {
  // after this, the use of kmalloc_page_align is FORBIDDEN
  // since heap-related data structures is placed right after the kernel
  heap = (heap_t *) kmalloc_page_align(sizeof(heap_t));
  kmemset(heap, 0, sizeof(heap_t));
  heap->start_addr = heap->end_addr = kernel_free_mem;
  kprintf("Heap starts at: 0x%x\n", heap->start_addr);
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
  return ret;
}

void *kmemset(void *ptr, int c, uint32_t n) {
  auto *p = (uint8_t *) ptr;
  for (uint32_t i = 0; i < n; ++i) {
    *p = c;
    ++p;
  }
  return ptr;
}

/**
 * @brief Create a new memory block and fill its header
 * @param addr the address of the new block's header
 * @param size size in bytes (not including the header)
 */
static void *new_memory_block(memory_block_header_t *prev, uint32_t addr, uint32_t size, bool used = true) {
  auto *header = (memory_block_header_t *) addr;

  header->prev = prev;
  header->size = size;
  header->used = used;

  uint32_t end_addr = addr + MEMORY_HEADER_SIZE + size;
  if (end_addr > heap->end_addr) {
    heap->end_addr = end_addr;
  }

  return (void *) (addr + MEMORY_HEADER_SIZE);
}

/**
 * @brief Reuse an existing memory block
 * @param addr the address of the block's header
 */
static void *reuse_memory_block(uint32_t addr) {
  auto *header = (memory_block_header_t *) addr;
  header->used = 1;
  return (void *) (addr + MEMORY_HEADER_SIZE);
}

memory_block_header_t *block_header_from_addr(void *ptr) {
  uint32_t addr = (uint32_t) ptr;
  kassert(addr >= MEMORY_HEADER_SIZE, "Invalid pointer");
  return (memory_block_header_t *) (addr - MEMORY_HEADER_SIZE);
}

/**
 * @brief Merge two free memory blocks if they are adjacent
 * @note The order of arguments matters! Also, only merge when they are both unused
 * @return The new block if merged, otherwise nullptr
 */
static memory_block_header_t *merge_blocks_if_possible(memory_block_header_t *prev, memory_block_header_t *next) {
  if (1 == prev->used || 1 == next->used) {
    return nullptr;
  }

  if (next->prev == prev) {
    prev->size += next->size + MEMORY_HEADER_SIZE;
  }
}

void *malloc(uint32_t size) {
  void *ret = nullptr;
  // find an available memory block descriptor
  uint32_t addr = heap->start_addr;
  memory_block_header_t *header = nullptr;
  while (addr < heap->end_addr) {
    header = (memory_block_header_t *) addr;
    // reuse the free block
    if (!header->used && header->size >= size) {
      // if there is available space for a new block and its header, split this block
      if (header->size >= size + MEMORY_HEADER_SIZE + MIN_MEMORY_SPLIT_THRESHOLD) {
        uint32_t extra_size = header->size - size - MEMORY_HEADER_SIZE;

        // reused the first part
        ret = new_memory_block(header->prev, addr, size);

        uint32_t new_addr = (uint32_t) ret + size;
        new_memory_block(header, new_addr, extra_size, false);
      } else {
        // otherwise just reuse this block
        ret = reuse_memory_block(addr);
      }
      break;
    }

    kassert(header->size, "Invalid memory block header");
    addr += header->size + MEMORY_HEADER_SIZE;
  }

  if (!ret) { // no existing free memory block can be reused
    ret = new_memory_block(header, heap->end_addr, size);
  }
  return ret;
}

void free(void *ptr) {
  kassert(ptr, "Cannot free a nullptr");
  memory_block_header_t *header = block_header_from_addr(ptr);
  header->used = 0;

  memory_block_header_t *other = header->prev;
  if (other)
    merge_blocks_if_possible(other, header);

  other = next_header(header);
  if (other) {
    merge_blocks_if_possible(header, other);
  }

  // heap.end_addr doesn't need to be reduced
}

memory_block_header_t *next_header(memory_block_header_t *header) {
  uint32_t next_addr = (uint32_t) header + MEMORY_HEADER_SIZE + header->size;
  if (next_addr < heap->end_addr) {
    return (memory_block_header_t *) (next_addr);
  }
  return nullptr;
}
