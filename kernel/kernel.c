#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "util.h"

void main() {
    clear_screen();

    isr_install();
    /* Test the interrupts */
    __asm__ __volatile__("int $0");
    __asm__ __volatile__("int $3");
}
