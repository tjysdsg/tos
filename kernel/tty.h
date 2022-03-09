#ifndef TOS_KERNEL_TTY_H_
#define TOS_KERNEL_TTY_H_
#include "multiboot.h"

#ifdef __cplusplus
extern "C" {
#endif

void init_tty(multiboot_info_t *mbi);

void putchar(char character);

/**
 * @brief Display input from keyboard
 */
void tty_show_keyboard_input(char character);

void clear_screen();

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_TTY_H_
