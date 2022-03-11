#ifndef TOS_KERNEL_MEMORY_H
#define TOS_KERNEL_MEMORY_H
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct memory_block_header {
  memory_block_header *prev;
  uint32_t size: 31; /// size of a memory block, not including the header
  uint32_t used: 1;
} memory_block_header_t;

typedef struct {
  /// the entire heap spans the range [start_addr, end_addr)
  uint32_t start_addr; /// start of heap (the first header)
  uint32_t end_addr;   /// current end of heap
} heap_t;

void init_memory();
void init_heap();

/**
 * @brief Get a 4KB-aligned free memory address
 * @param size size in bytes, padded by 4KB
 * @note Undefined behavior if memory outside of the allocated range is modified
 */
uint32_t kmalloc_page_align(uint32_t size);

void *kmemset(void *ptr, int c, uint32_t n);

void *malloc(uint32_t size);
void free(void *ptr);

/**
 * @brief Get header of the memory block referred by a pointer
 * @param ptr The start of the memory block (not the header)
 */
memory_block_header_t *block_header_from_addr(void *ptr);

/**
 * @brief Find the next block header
 * @return nullptr if header is already the last header on heap
 */
memory_block_header_t *next_header(memory_block_header_t *header);

/**
 * @brief Map a segment of physical memory to virtual memory
 * @details This will create new virtual page(s) if needed
 * @param addr Starting physical address
 * @param size Length of the segment
 * @return The corresponding address of addr, in virtual memory
 */
uint32_t phys2virt(uint32_t addr, uint32_t size);

/**
 * @brief Find the corresponding physical memory address of a virtual address
 */
uint32_t virt2phys(uint32_t addr);

#ifdef __cplusplus
}

/// Overloading new and delete operators

void *operator new(size_t sz);
void *operator new[](size_t sz);
void operator delete(void *ptr, size_t sz) noexcept;
void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;

#endif

#endif //TOS_KERNEL_MEMORY_H
