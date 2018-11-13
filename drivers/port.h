#ifndef PORT_H
#define PORT_H
#include "../common/types.h"

byte_t port_byte_in(byte_t port);
void port_byte_out(byte_t port, byte_t data);
word_t port_word_in(byte_t port);
void port_word_out(byte_t port, word_t data);

#endif
