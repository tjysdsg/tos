#ifndef TOS_KERNEL_TTY_H_
#define TOS_KERNEL_TTY_H_

// attribute of an character
#define ATTRIBUTE               7
// video memory address
#define VIDEO                   0xB8000

extern "C" {

void putchar(char character);

void clear_screen();

};

#endif //TOS_KERNEL_TTY_H_
