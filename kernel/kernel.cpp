#include "multiboot.h"
#include "tty.h"
#include "kprintf.h"
#include "gdt.h"
#include "idt.h"
#include "kpanic.h"

extern uint8_t _kernel_seg_end;

extern "C" void kmain(unsigned long addr) {
  auto *mbi = (multiboot_info_t *) addr;

  init_tty(mbi);
  clear_screen();

  kassert(gdt_initialized, "GDT is not initialized");

  init_idt();

  { // find kernel memory start address, align by 4KB
    auto kernel_seg_end = (uint32_t) &_kernel_seg_end;
    uint32_t remain = kernel_seg_end % 4096;
    uint32_t fill = 4096 - remain;
    kernel_seg_end += fill;
    kassert(kernel_seg_end % 4096 == 0, "Cannot find kernel memory starting address");
    kprintf("Kernel memory start = 0x%x\n", kernel_seg_end);
  }

  // test idt
  /*
  asm volatile ("int $3");
  asm volatile ("int $10");
  asm volatile ("int $31");
  */

  /// 2. Print multiboot header and multiboot information
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
  if (CHECK_FLAG(mbi->flags, 0))
    kprintf("  mem_lower = %uKB, mem_upper = %uKB\n", (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

  // boot_device
  if (CHECK_FLAG(mbi->flags, 1))
    kprintf("  boot_device = 0x%x\n", (unsigned) mbi->boot_device);

  // cmdline
  if (CHECK_FLAG(mbi->flags, 2))
    kprintf("  cmdline = %s\n", (char *) mbi->cmdline);

  // mods_*
  if (CHECK_FLAG(mbi->flags, 3)) {
    multiboot_module_t *mod;
    int i;

    kprintf("  mods_count = %d, mods_addr = 0x%x\n", (int) mbi->mods_count, (int) mbi->mods_addr);
    for (i = 0, mod = (multiboot_module_t *) mbi->mods_addr; i < mbi->mods_count; i++, mod++)
      kprintf(
          "    mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
          (unsigned) mod->mod_start,
          (unsigned) mod->mod_end,
          (char *) mod->cmdline
      );
  }

  // symbol table?
  if (CHECK_FLAG(mbi->flags, 4)) {
    multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);

    kprintf(
        "  multiboot_aout_symbol_table: tabsize = 0x%0x, strsize = 0x%x, addr = 0x%x\n",
        (unsigned) multiboot_aout_sym->tabsize,
        (unsigned) multiboot_aout_sym->strsize,
        (unsigned) multiboot_aout_sym->addr
    );
  }

  // section header table being ELF valid?
  if (CHECK_FLAG(mbi->flags, 5)) {
    multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

    kprintf(
        "  multiboot_elf_sec: num = %u, size = 0x%x, addr = 0x%x, shndx = 0x%x\n",
        (unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
        (unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->shndx
    );
  }

  // mmap_*
  if (CHECK_FLAG(mbi->flags, 6)) {
    multiboot_memory_map_t *mmap;

    kprintf("  mmap_addr = 0x%x, mmap_length = 0x%x\n", (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
    for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
         (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
         mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size)))
      kprintf(
          "    size = 0x%x, base_addr = 0x%x%08x, length = 0x%x%08x, type = 0x%x\n",
          (unsigned) mmap->size,
          (unsigned) (mmap->addr >> 32),
          (unsigned) (mmap->addr & 0xffffffff),
          (unsigned) (mmap->len >> 32),
          (unsigned) (mmap->len & 0xffffffff),
          (unsigned) mmap->type
      );
  }
}
