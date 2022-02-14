MBOOT_AOUT_KLUDGE   equ 0      ; GRUB does not pass us a symbol table
MBOOT_PAGE_ALIGN    equ 0b1    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 0b10   ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_VIDEO_MODE    equ 0x00000004
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_VIDEO_MODE | MBOOT_AOUT_KLUDGE
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]

global multiboot_header
extern _kernel_seg_code ; see linker.ld
extern _kernel_seg_bss  ; see linker.ld
extern _kernel_seg_end  ; see linker.ld

multiboot_header:
  dd MBOOT_HEADER_MAGIC
  dd MBOOT_HEADER_FLAGS
  dd MBOOT_CHECKSUM

  dd multiboot_header    ; header_addr
  dd _kernel_seg_code    ; load_addr
  dd _kernel_seg_bss     ; load_end_addr
  dd _kernel_seg_end     ; bss_end_addr.
  dd start               ; entry_addr

  ; dd 1                         ; graphics mode type. 0 for linear graphics mode, 1 for EGA-standard text mode
  ; dd 80                        ; width
  ; dd 24                        ; height
  ; dd 0                         ; depth

  dd 0
  dd 800
  dd 600
  dd 32

global start
extern load_gdt
extern kmain                 ; see kernel.cpp

start:
  cli  ; disable interrupts until IDT is loaded
  mov  esp, stack_end  ; custom stack

  call load_gdt

  push    ebx                  ; Load multiboot header location
  call kmain                   ; call our main() function
  jmp $

SECTION .bss

stack_begin:
    resb 0x1000000  ; 1MB for stack
stack_end:
