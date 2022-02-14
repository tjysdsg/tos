global flush_idt

flush_idt:
   mov eax, [esp+4]  ; IDTR
   lidt [eax]
   ret

; cli and sti are not required in interrupt handlers

; interrupt service routines
%macro ISR_NOERR_HANDLER 1  ; 0 as error code (cuz there's no error)
    [GLOBAL isr%1]
isr%1:
    push byte 0
    push byte %1
    pushad ; edi,esi,ebp,esp,ebx,edx,ecx,eax
    cld
    call isr_handler
    popad

    add esp, 8
    iret
%endmacro

%macro ISR_ERR_HANDLER 1 ; error code is already set by CPU
    [GLOBAL isr%1]
isr%1:
    push byte %1
    pushad ; edi,esi,ebp,esp,ebx,edx,ecx,eax
    cld
    call isr_handler
    popad

    add esp, 8
    iret
%endmacro

; creates a stub for an IRQ
; args:
;   1. the IRQ number
;   2. the ISR number it is remapped to
%macro IRQ 2
  global irq%1
  irq%1:
    push byte 0
    push byte %2
    pushad ; edi,esi,ebp,esp,ebx,edx,ecx,eax
    cld
    call irq_handler
    popad

    add esp, 8
    iret
%endmacro


extern isr_handler  ; see isr.cpp
extern irq_handler  ; see isr.cpp

ISR_NOERR_HANDLER 0
ISR_NOERR_HANDLER 1
ISR_NOERR_HANDLER 2
ISR_NOERR_HANDLER 3
ISR_NOERR_HANDLER 4
ISR_NOERR_HANDLER 5
ISR_NOERR_HANDLER 6
ISR_NOERR_HANDLER 7
ISR_NOERR_HANDLER 8
ISR_NOERR_HANDLER 9
ISR_NOERR_HANDLER 10
ISR_NOERR_HANDLER 11
ISR_NOERR_HANDLER 12
ISR_NOERR_HANDLER 13
ISR_NOERR_HANDLER 14
ISR_NOERR_HANDLER 15
ISR_NOERR_HANDLER 16
ISR_ERR_HANDLER 17
ISR_NOERR_HANDLER 18
ISR_NOERR_HANDLER 19
ISR_NOERR_HANDLER 20
ISR_ERR_HANDLER 21
ISR_NOERR_HANDLER 22
ISR_NOERR_HANDLER 23
ISR_NOERR_HANDLER 24
ISR_NOERR_HANDLER 25
ISR_NOERR_HANDLER 26
ISR_NOERR_HANDLER 27
ISR_NOERR_HANDLER 28
ISR_NOERR_HANDLER 29
ISR_NOERR_HANDLER 30
ISR_NOERR_HANDLER 31

IRQ   0,  32
IRQ   1,  33
IRQ   2,  34
IRQ   3,  35
IRQ   4,  36
IRQ   5,  37
IRQ   6,  38
IRQ   7,  39
IRQ   8,  40
IRQ   9,  41
IRQ   10, 42
IRQ   11, 43
IRQ   12, 44
IRQ   13, 45
IRQ   14, 46
IRQ   15, 47
