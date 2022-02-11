global flush_idt

flush_idt:
   mov eax, [esp+4]  ; IDTR
   lidt [eax]
   ret


; interrupt service routines
%macro ISR_INT_HANDLER 1  ; 0 as error code (cuz there's no error)
    [GLOBAL isr%1]
isr%1:
    cli
    push byte 0
    push byte %1
    pushad ; edi,esi,ebp,esp,ebx,edx,ecx,eax
    call isr_handler
    popad

    add esp, 8
    sti
    iret
%endmacro

%macro ISR_EXCEPTION_HANDLER 1 ; error code is already set by CPU
    [GLOBAL isr%1]
isr%1:
    cli
    push byte %1
    pushad ; edi,esi,ebp,esp,ebx,edx,ecx,eax
    call isr_handler
    popad

    add esp, 8
    sti
    iret
%endmacro

extern isr_handler  ; see idt.cpp

ISR_INT_HANDLER 0
ISR_INT_HANDLER 1
ISR_INT_HANDLER 2
ISR_INT_HANDLER 3
ISR_INT_HANDLER 4
ISR_INT_HANDLER 5
ISR_INT_HANDLER 6
ISR_INT_HANDLER 7
ISR_INT_HANDLER 8
ISR_INT_HANDLER 9
ISR_INT_HANDLER 10
ISR_INT_HANDLER 11
ISR_INT_HANDLER 12
ISR_INT_HANDLER 13
ISR_INT_HANDLER 14
ISR_INT_HANDLER 15
ISR_INT_HANDLER 16
ISR_INT_HANDLER 17
ISR_INT_HANDLER 18
ISR_INT_HANDLER 19
ISR_INT_HANDLER 20
ISR_INT_HANDLER 21
ISR_INT_HANDLER 22
ISR_INT_HANDLER 23
ISR_INT_HANDLER 24
ISR_INT_HANDLER 25
ISR_INT_HANDLER 26
ISR_INT_HANDLER 27
ISR_INT_HANDLER 28
ISR_INT_HANDLER 29
ISR_INT_HANDLER 30
ISR_INT_HANDLER 31
