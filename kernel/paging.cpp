#include "string.h"
#include "paging.h"
#include "memory.h"
#include "kpanic.h"
#include "kprintf.h"
#include "isr.h"

page_directory_t *kernel_page_directory = nullptr;

extern "C" void load_page_directory(uint32_t page_directory);
extern "C" void enable_paging();

static void page_fault(registers_t *regs) {
  // the faulting address is stored in the CR2 register.
  uint32_t faulting_address;
  asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

  // The error code gives us details of what happened.
  int present = !(regs->err_code & 0x1); // Page not present
  int rw = regs->err_code & 0x2;         // Write operation?
  int us = regs->err_code & 0x4;         // Processor was in user-mode?
  int reserved = regs->err_code & 0x8;   // Overwritten CPU-reserved bits of page entry?
  int id = regs->err_code & 0x10;        // Caused by an instruction fetch?

  // Output an error message.
  kprintf("Page fault. Address: 0x%x. Error reason: ", faulting_address);
  if (present) { kprintf("not present\n"); }
  if (rw) { kprintf("read-only\n"); }
  if (us) { kprintf("user-mode\n"); }
  if (id) { kprintf("inst-fetch\n"); }
  if (reserved) { kprintf("reserved\n"); }

  kpanic("See above");
}

void init_paging() {
  /// init page directory
  kernel_page_directory = (page_directory_t *) kmalloc_page_align(sizeof(page_directory_t));
  memset(kernel_page_directory, 0, sizeof(page_directory_t));

  /// init 1024 page entries and fill their addresses into the corresponding page directory
  auto *kernel_page_tables = (page_table_t *) kmalloc_page_align(sizeof(page_table_t) * 1024);
  memset(kernel_page_tables, 0, sizeof(page_table_t) * 1024);

  /// create an identity map of the entire 4GB physical memory, used by kernel
  uint32_t addr = 0;
  for (int tab_i = 0; tab_i < 1024; ++tab_i) {
    // write enabled, present
    kernel_page_directory->entries[tab_i].rw = 1;
    kernel_page_directory->entries[tab_i].present = 1;
    kernel_page_directory->entries[tab_i].addr = ((uint32_t) &kernel_page_tables[tab_i]) >> 12;

    for (int page_i = 0; page_i < 1024; ++page_i, addr += 0x1000) {
      // write enabled, present
      // kernel_page_tables[tab_i].pages[page_i].user = 1;
      kernel_page_tables[tab_i].pages[page_i].rw = 1;
      kernel_page_tables[tab_i].pages[page_i].present = 1;
      kernel_page_tables[tab_i].pages[page_i].addr = addr >> 12;

      kassert((addr >> 12) == ((tab_i << 10) + page_i), "Invalid page physical address");
    }
  }

  register_interrupt_handler(14, page_fault);

  kprintf("Loaded page directories and tables\n");

  load_page_directory((uint32_t) kernel_page_directory);
  enable_paging();
}
