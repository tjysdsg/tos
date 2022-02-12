#ifndef TOS_KERNEL_MULTIBOOT_H
#define TOS_KERNEL_MULTIBOOT_H
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

// check if the bit BIT in FLAGS is set
#define CHECK_FLAG(flags, bit)   ((flags) & (1 << (bit)))

typedef struct {
  /* Must be MULTIBOOT_MAGIC */
  uint32_t magic;

  /* Feature flags. */
  uint32_t flags;

  /* The above fields plus this one must equal 0 mod 2^32. */
  uint32_t checksum;

  /* These are only valid if MULTIBOOT_AOUT_KLUDGE is set. */
  uint32_t header_addr;
  uint32_t load_addr;
  uint32_t load_end_addr;
  uint32_t bss_end_addr;
  uint32_t entry_addr;

  /* These are only valid if MULTIBOOT_VIDEO_MODE is set. */
  uint32_t mode_type;
  uint32_t width;
  uint32_t height;
  uint32_t depth;
} multiboot_header_t;

extern multiboot_header_t multiboot_header;

/* The symbol table for a.out. */
typedef struct {
  uint32_t tabsize;
  uint32_t strsize;
  uint32_t addr;
  uint32_t reserved;
} multiboot_aout_symbol_table_t;

/* The section header table for ELF. */
typedef struct {
  uint32_t num;
  uint32_t size;
  uint32_t addr;
  uint32_t shndx;
} multiboot_elf_section_header_table_t;

typedef struct {
  /* Multiboot info version number */
  uint32_t flags;

  /* Available memory from BIOS */
  uint32_t mem_lower;
  uint32_t mem_upper;

  /* "root" partition */
  uint32_t boot_device;

  /* Kernel command line */
  uint32_t cmdline;

  /* Boot-Module list */
  uint32_t mods_count;
  uint32_t mods_addr;

  union {
    multiboot_aout_symbol_table_t aout_sym;
    multiboot_elf_section_header_table_t elf_sec;
  } u;

  /* Memory Mapping buffer */
  uint32_t mmap_length;
  uint32_t mmap_addr;

  /* Drive Info buffer */
  uint32_t drives_length;
  uint32_t drives_addr;

  /* ROM configuration table */
  uint32_t config_table;

  /* Boot Loader Name */
  uint32_t boot_loader_name;

  /* APM table */
  uint32_t apm_table;

  /* Video */
  uint32_t vbe_control_info;
  uint32_t vbe_mode_info;
  uint16_t vbe_mode;
  uint16_t vbe_interface_seg;
  uint16_t vbe_interface_off;
  uint16_t vbe_interface_len;

  uint64_t framebuffer_addr;
  uint32_t framebuffer_pitch;
  uint32_t framebuffer_width;
  uint32_t framebuffer_height;
  uint8_t framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB     1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT     2
  uint8_t framebuffer_type;
  union {
    struct {
      uint32_t framebuffer_palette_addr;
      uint16_t framebuffer_palette_num_colors;
    };
    struct {
      uint8_t framebuffer_red_field_position;
      uint8_t framebuffer_red_mask_size;
      uint8_t framebuffer_green_field_position;
      uint8_t framebuffer_green_mask_size;
      uint8_t framebuffer_blue_field_position;
      uint8_t framebuffer_blue_mask_size;
    };
  };
} multiboot_info_t;

typedef struct {
  uint32_t size;
  uint64_t addr;
  uint64_t len;
#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5
  uint32_t type;
} __attribute__((packed)) multiboot_memory_map_t;

typedef struct {
  /* the memory used goes from bytes ’mod_start’ to ’mod_end-1’ inclusive */
  uint32_t mod_start;
  uint32_t mod_end;

  /* Module command line */
  uint32_t cmdline;

  /* padding to take it to 16 bytes (must be zero) */
  uint32_t pad;
} multiboot_module_t;

/* APM BIOS info. */
typedef struct {
  uint16_t version;
  uint16_t cseg;
  uint32_t offset;
  uint16_t cseg_16;
  uint16_t dseg;
  uint16_t flags;
  uint16_t cseg_len;
  uint16_t cseg_16_len;
  uint16_t dseg_len;
} multiboot_apm_info_t;

#ifdef __cplusplus
}
#endif

#endif /* TOS_KERNEL_MULTIBOOT_H */
