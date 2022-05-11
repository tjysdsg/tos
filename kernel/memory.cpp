#include <stdint.h>
#include "multiboot.h"
#include "kpanic.h"
#include "memory.h"
#include "string.h"
#include "kprintf.h"
#include "isr.h"

#define MEMORY_HEADER_SIZE sizeof(memory_block_header_t)

/// split a free memory block into two if there is at least some extra space with this size
#define MIN_MEMORY_SPLIT_THRESHOLD 4

extern uint8_t _kernel_seg_end; /// see linker.ld

static uint32_t kernel_free_mem = 0; /// current starting address of free memory
static heap_t *heap = nullptr;
static bool heap_initialized = false;

multiboot_memory_map_t multiboot_mmaps[128];

/// https://wiki.osdev.org/Exceptions#General_Protection_Fault
static void general_protection_fault_handler(registers_t *regs) {
  kprintf("General protection fault, segment selector index = 0x%x\n", regs->err_code);
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
void init_memory(uint32_t mmap_addr, uint32_t mmap_length) {
  kernel_free_mem = (uint32_t) &(_kernel_seg_end);

  { // find an available memory segment (after the kernel itself) in the memory map as the location of heap
    memset(multiboot_mmaps, 0, sizeof(multiboot_mmaps));

    multiboot_memory_map_t *mmap = nullptr;
    uint32_t available_mem_start = 0;
    uint32_t available_mem_len = 0;
    int i = 0;
    for (
        mmap = (multiboot_memory_map_t *) mmap_addr;
        (uint32_t) mmap < mmap_addr + mmap_length;
        mmap = (multiboot_memory_map_t *) ((uint32_t) mmap + mmap->size + sizeof(mmap->size))
        ) {
      // 32 bit system so only need uint32_t
      uint32_t start_addr = mmap->addr;
      uint32_t length = mmap->len;
      uint32_t type = mmap->type;
      uint32_t end_addr = start_addr + length;

      // save the memory maps for easier access in the future
      multiboot_mmaps[i].addr = start_addr;
      multiboot_mmaps[i].len = length;
      multiboot_mmaps[i].type = type;

      kprintf(
          "    base_addr = 0x%08x, length = 0x%08x, type = %d\n",
          (uint32_t) (start_addr),
          (uint32_t) (length),
          (uint32_t) type
      );

      uint32_t candidate_start = 0;
      uint32_t candidate_length = 0;
      if (type != MULTIBOOT_MEMORY_AVAILABLE) continue;

      if (start_addr <= kernel_free_mem && end_addr > kernel_free_mem) {
        candidate_start = kernel_free_mem;
        candidate_length = length - (kernel_free_mem - start_addr);
      } else if (start_addr > kernel_free_mem) {
        candidate_start = start_addr;
        candidate_length = length;
      }

      // prefer using a larger memory segment
      if (candidate_length > available_mem_len) {
        kprintf("Found available memory at 0x%x, length 0x%x\n", candidate_start, candidate_length);
        available_mem_start = candidate_start;
        available_mem_len = candidate_length;
      }

      ++i;
    }

    kassert(available_mem_start && available_mem_len, "Cannot find available memory in multiboot mmap");
    kernel_free_mem = available_mem_start;
  }

  kernel_free_mem = align_addr_by_4k(kernel_free_mem);
  kprintf("Kernel free memory starts at: 0x%x\n", kernel_free_mem);

  // register memory-related interrupt handler
  register_interrupt_handler(13, general_protection_fault_handler);
}

void init_heap() {
  // after this, the use of kmalloc_page_align is FORBIDDEN
  // since heap-related data structures is placed right after the kernel
  heap = (heap_t *) kmalloc_page_align(sizeof(heap_t));
  memset(heap, 0, sizeof(heap_t));
  heap->start_addr = heap->end_addr = kernel_free_mem;
  kprintf("Heap starts at: 0x%x\n", heap->start_addr);

  heap_initialized = true;
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
  return prev;
}

void *malloc(uint32_t size) {
  kassert(heap_initialized, "The heap is not initialized");
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
  kassert(heap_initialized, "The heap is not initialized");
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

void *operator new(size_t sz) {
  return malloc(sz);
}

void operator delete(void *ptr) noexcept {
  free(ptr);
}

void *operator new[](size_t sz) {
  return malloc(sz);
}

void operator delete[](void *ptr) noexcept {
  free(ptr);
}

void operator delete(void *ptr, size_t sz) noexcept {
  free(ptr);
}

uint32_t phys2virt(uint32_t addr, uint32_t size) {
  // FIXME: for kernel, the address is the same. But this is not true in userland
  return addr;
}

uint32_t virt2phys(uint32_t addr) {
  // FIXME: for kernel, the address is the same. But this is not true in userland
  return addr;
}
