#include "multiboot.h"
#include "tty.h"
#include "kprintf.h"
#include "gdt.h"
#include "pic.h"
#include "idt.h"
#include "kpanic.h"
#include "memory.h"
#include "paging.h"
#include "pit.h"
#include "ps2_keyboard.h"
#include "acpi_driver.h"
#include "pci.h"

#ifdef __TOS_ENABLE_KERNEL_TESTS__
#include "kernel_test.h"
#endif

extern "C" void kmain(unsigned long addr) {
  auto *mbi = (multiboot_info_t *) addr;

  // must be the first thing that is called, since even kpanic relies on printing to console
  init_tty(mbi);
  clear_screen();

  // init memory map
  kassert(MULTIBOOT_CHECK_FLAG(mbi->flags, 6), "Multiboot must give us memory map");
  init_memory(mbi->mmap_addr, mbi->mmap_length);

  // TODO: save all contents in mbi here, since multiboot somehow placed things in memory it itself marked as available

  // check if gdt is initialized before kmain is called, in gdt.asm
  kassert(gdt_initialized, "GDT is not initialized");

  // initialize APIC and interrupt handlers
  remap_pic();
  init_apic();
  init_idt();

  init_pit(119318); // PIT is used to calibrate APIC timer, so init first
  init_apic_timer();

  init_ps2_keyboard();

  // initialize memory related components
  init_paging();
  init_heap();

#ifdef __TOS_ENABLE_KERNEL_TESTS__
  malloc_test(); // NOTE: malloc test must be placed before any malloc() calls
#endif

  // ACPI, must be after init_heap, since it uses dynamic memory allocation
  init_acpi();

  init_pci();

  // enable interrupts
  enable_interrupt();

  // calibrate APIC timer
  calibrate_apic_timer(100);

#ifdef __TOS_ENABLE_KERNEL_TESTS__
  interrupt_test();
  pit_test();
#endif

  /*
  // print multiboot header and multiboot information
  kprintf("multiboot header:\n");
  kprintf("  flags = 0x%x\n", multiboot_header.flags);
  kprintf("  header_addr = 0x%x\n", multiboot_header.header_addr);
  kprintf("  load_addr = 0x%x\n", multiboot_header.load_addr);
  kprintf("  load_end_addr = 0x%x\n", multiboot_header.load_end_addr);
  kprintf("  bss_end_addr = 0x%x\n", multiboot_header.bss_end_addr);
  kprintf("  entry_addr = 0x%x\n", multiboot_header.entry_addr);
  kprintf("  mode_type = %d\n", multiboot_header.mode_type);
  kprintf("  width = %d\n", multiboot_header.width);
  kprintf("  height = %d\n", multiboot_header.height);
  kprintf("  depth = %d\n", multiboot_header.depth);

  /// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
  kprintf("multiboot info:\n");
  kprintf("  flags = 0x%x\n", (unsigned) mbi->flags);

  // mem_lower and mem_upper
  if (MULTIBOOT_CHECK_FLAG(mbi->flags, 0))
    kprintf("  mem_lower = %uKB, mem_upper = %uKB\n", (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

  // boot_device
  if (MULTIBOOT_CHECK_FLAG(mbi->flags, 1))
    kprintf("  boot_device = 0x%x\n", (unsigned) mbi->boot_device);

  // cmdline
  if (MULTIBOOT_CHECK_FLAG(mbi->flags, 2))
    kprintf("  cmdline = %s\n", (char *) mbi->cmdline);

  // mods_*
  if (MULTIBOOT_CHECK_FLAG(mbi->flags, 3)) {
    kprintf("  mods_count = %d, mods_addr = 0x%x\n", (int) mbi->mods_count, (int) mbi->mods_addr);

    uint32_t i;
    multiboot_module_t *mod;
    for (i = 0, mod = (multiboot_module_t *) mbi->mods_addr; i < mbi->mods_count; i++, mod++)
      kprintf(
          "    mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
          (unsigned) mod->mod_start,
          (unsigned) mod->mod_end,
          (char *) mod->cmdline
      );
  }

  // symbol table?
  if (MULTIBOOT_CHECK_FLAG(mbi->flags, 4)) {
    multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);

    kprintf(
        "  multiboot_aout_symbol_table: tabsize = 0x%0x, strsize = 0x%x, addr = 0x%x\n",
        (unsigned) multiboot_aout_sym->tabsize,
        (unsigned) multiboot_aout_sym->strsize,
        (unsigned) multiboot_aout_sym->addr
    );
  }

  // section header table being ELF valid?
  if (MULTIBOOT_CHECK_FLAG(mbi->flags, 5)) {
    multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

    kprintf(
        "  multiboot_elf_sec: num = %u, size = 0x%x, addr = 0x%x, shndx = 0x%x\n",
        (unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
        (unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->shndx
    );
  }
  */
}
