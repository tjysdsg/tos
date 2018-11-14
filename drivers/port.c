#include "port.h"

/**
 * Read a byte from the specified port
 */
byte_t port_byte_in(byte_t port) {
	byte_t result;
	/* Inline assembler syntax
	 * '"=a" (result)'; set '=' the C variable '(result)' to the value of register
	 * e'a'x
	 * '"d" (port)': map the C variable '(port)' into e'd'x register
	 * Inputs and outputs are separated by colons
	 */
	__asm__("in %%dx, %%al"
	: "=a"(result)
	: "d"(port));
	return result;
}

void port_byte_out(byte_t port, byte_t data) {
	__asm__("out %%al, %%dx"
	:
	: "a"(data), "d"(port));
}

word_t port_word_in(byte_t port) {
	unsigned short result;
	__asm__("in %%dx, %%ax"
	: "=a"(result)
	: "d"(port));
	return result;
}

void port_word_out(byte_t port, word_t data) {
	__asm__("out %%ax, %%dx"
	:
	: "a"(data), "d"(port));
}
