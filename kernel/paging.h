#ifndef TOS_KERNEL_PAGING_H
#define TOS_KERNEL_PAGING_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// https://wiki.osdev.org/Paging

typedef struct {
  uint32_t present: 1;  // page present in memory
  uint32_t rw: 1;       // read-only if clear, readwrite if set
  uint32_t user: 1;     // supervisor level only if clear
  uint32_t accessed: 1; // has the page been accessed since last refresh?
  uint32_t dirty: 1;    // has the page been written to since last refresh?
  uint32_t unused: 7;   // not used by processor, os can store custom values
  uint32_t addr: 20;    // page physical address, shifted right 12 bits
} __attribute__((packed)) page_table_entry_t;

typedef struct {
  page_table_entry_t pages[1024];
} page_table_t;

// 4KB aligned page directory entry
typedef struct {
  uint32_t present: 1;
  uint32_t rw: 1;      // 0 = read only, 1 = readable+writable
  uint32_t us: 1;      // 0 = supervisor, 1 = all user
  uint32_t pwt: 1;     // 0 = write back enabled, 1 = write-through caching enabled
  uint32_t pcd: 1;     // 1 = page is not cached
  uint32_t a: 1;       // accessed
  uint32_t unused1: 1; // unused
  uint32_t ps: 1;      // page size, 0 for 4KB, 1 for 4MB
  uint32_t unused2: 4; // unused
  uint32_t addr: 20;   // address of page table, shifted right 12 bits
} __attribute__((packed)) page_directory_entry_t;

typedef struct {
  page_directory_entry_t entries[1024];
} page_directory_t;

extern page_directory_t *kernel_page_directory;

void init_paging();

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_PAGING_H
