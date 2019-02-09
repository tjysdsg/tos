gdt_start:
    ; the GDT must starts with a null 8-byte
    dd 0x0 ; 4 byte
    dd 0x0 ; 4 byte

; GDT for code segment. base = 0x00000000, length = 0xfffff
gdt_code:
    dw 0xffff    ; segment length, bits 0-15

    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23

    db 10011010b ; access byte (8 bits)
                 ; DETAILS:
                 ;     access bit=0;
                 ;     readable
                 ;     code in this segment can only be executed from the ring set in priviledge bits
                 ;     executable bit=1 (is a code segment)
                 ;     descriptor type=1 (either code or data segment)
                 ;     ring 0
                 ;     present bit=1

    db 11001111b ; flags (4 bits) + the 16th to 19th bits of segment length (4 bits)
                 ; DETAILS:
                 ;     page granularity
                 ;     32 bit protected mode

    db 0x0       ; segment base, bits 24-31

; GDT for data segment. base = 0x00000000, length = 0xfffff
gdt_data:
    dw 0xffff    ; segment length, bits 0-15

    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23

    db 10010010b ; access byte (8 bits)
                 ; DETAILS:
                 ;     access bit=0;
                 ;     readable
                 ;     data segment grows up
                 ;     executable bit=0 (is a data segment)
                 ;     descriptor type=1 (either code or data segment)
                 ;     ring 0
                 ;     present bit=1

    db 11001111b ; flags (4 bits) + the 16th to 19th bits of segment length (4 bits)
                 ; DETAILS:
                 ;     page granularity
                 ;     32 bit protected mode

    db 0x0       ; segment base, bits 24-31

gdt_end:

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size (16 bit), always one less of its true size
    dd gdt_start ; address (32 bit)

; define some constants for later use
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
