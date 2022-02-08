#include <stddef.h>
#include <stdint.h>
#include "multiboot.h"
#include "gdt.h"
#include "tty.h"
#include "kprintf.h"
#include "FrameBuffer.h"

// check if the bit BIT in FLAGS is set
#define CHECK_FLAG(flags, bit)   ((flags) & (1 << (bit)))

static void init_vbe(multiboot_info_t *mbi) {
  if (CHECK_FLAG(mbi->flags, 12)) {
    // TODO: report error
    return;
  }
}

extern "C" void kmain(unsigned long magic, unsigned long addr) {
  multiboot_info_t *mbi;

  clear_screen();

  /// 1. Check if MAGIC indicates a valid multiboot header
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    kprintf("Invalid magic number: 0x%x\n", (unsigned) magic);
    return;
  }

  // set mbi to the address of the Multiboot information structure
  mbi = (multiboot_info_t *) addr;

  // TODO: implement font drawing using VBE
  //  init_vbe(mbi);
  //  FrameBuffer fb(mbi->framebuffer_type,
  //                 mbi->framebuffer_bpp,
  //                 mbi->framebuffer_pitch,
  //                 reinterpret_cast<uint8_t *>(mbi->framebuffer_addr));
  //  for (int i = 0; i < 500; ++i) {
  //    fb.put_pixel(i, i);
  //  }

  /*
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
  */

  init_gdt();
}
