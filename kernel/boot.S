#define ASM_FILE
#include "multiboot.h"

// stack size is 16KB
#define STACK_SIZE  0x4000

/* Multiboot flags */
#ifdef __ELF__
# define AOUT_KLUDGE 0
#else
# define AOUT_KLUDGE MULTIBOOT_AOUT_KLUDGE
#endif
#define MULTIBOOT_HEADER_FLAGS MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_VIDEO_MODE | AOUT_KLUDGE

.text
        .globl  start, _start

start:
_start:
        jmp multiboot_entry

        // Align by 32 bits
        .align  4

// Multiboot header
multiboot_header:
        // magic
        .long MULTIBOOT_HEADER_MAGIC
        // flags
        .long MULTIBOOT_HEADER_FLAGS
        // checksum
        .long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
#ifndef __ELF__
        // header_addr
        .long   multiboot_header
        // load_addr
        .long   _start
        // load_end_addr
        .long   _edata
        // bss_end_addr
        .long   _end
        // entry_addr
        .long   multiboot_entry
#else
        .long   0
        .long   0
        .long   0
        .long   0
        .long   0
#endif
        .long 1             # graphics mode type. 0 for linear graphics mode, 1 for EGA-standard text mode.
        .long TERM_COLUMNS  # width
        .long TERM_LINES    # height
        .long 0             # depth

        # .long 0     # graphics mode type. 0 for linear graphics mode, 1 for EGA-standard text mode.
        # .long 1024  # width
        # .long 768   # height
        # .long 32    # depth

multiboot_entry:
        // Initialize the stack pointer
        movl $(stack + STACK_SIZE), %esp

        // Reset EFLAGS
        pushl $0
        popf

        // multiboot_info
        pushl %ebx
        // magic value
        pushl %eax

        // kmain(magic, multiboot_info)
        call    kmain

        // Halt
        pushl   $halt_message
        call    kprintf

loop:   hlt
        jmp     loop

halt_message:
        .asciz  "System halt"

        // stack
        .comm   stack, STACK_SIZE
