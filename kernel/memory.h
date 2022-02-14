#ifndef TOS_KERNEL_MEMORY_H
#define TOS_KERNEL_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

void init_memory();

/**
 * @brief Get a 4KB-aligned free memory address
 * @param size size in bytes, padded by 4KB
 * @note Undefined behavior if memory outside of the allocated range is modified
 */
uint32_t kmalloc_page_align(uint32_t size);

/// https://man7.org/linux/man-pages/man3/memset.3.html
void *kmemset(void *ptr, int c, uint32_t n);

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_MEMORY_H
