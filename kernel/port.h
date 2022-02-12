#ifndef TOS_KERNEL_PORT_H
#define TOS_KERNEL_PORT_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void outb(uint16_t port, uint8_t value);

uint8_t inb(uint16_t port);

uint16_t inw(uint16_t port);

#ifdef __cplusplus
}
#endif

#endif //TOS_KERNEL_PORT_H
