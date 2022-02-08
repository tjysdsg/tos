MBOOT_AOUT_KLUDGE   equ 0      ; GRUB does not pass us a symbol table
MBOOT_PAGE_ALIGN    equ 0b1    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 0b10   ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_VIDEO_MODE    equ 0x00000004
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_VIDEO_MODE | MBOOT_AOUT_KLUDGE
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]

[GLOBAL multiboot_header]
[EXTERN code]                   ; see linker.ld
[EXTERN bss]                    ; see linker.ld
[EXTERN end]                    ; see linker.ld

multiboot_header:
  dd MBOOT_HEADER_MAGIC
  dd MBOOT_HEADER_FLAGS
  dd MBOOT_CHECKSUM

  dd multiboot_header          ; header_addr
  dd code                      ; load_addr
  dd bss                       ; load_end_addr
  dd end                       ; bss_end_addr.
  dd start                     ; entry_addr
  dd 1                         ; graphics mode type. 0 for linear graphics mode, 1 for EGA-standard text mode
  dd 80                        ; width
  dd 24                        ; height
  dd 0                         ; depth
  ; dd 0
  ; dd 1024
  ; dd 768
  ; dd 32

[GLOBAL start]
[EXTERN kmain]                 ; see kernel.cpp

start:
  push    ebx                  ; Load multiboot header location

  cli                          ; Disable interrupts
  call kmain                   ; call our main() function
  hlt
