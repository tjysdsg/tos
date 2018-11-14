#ifndef TOS_UTIL_H
#define TOS_UTIL_H
#include "../common/types.h"

void memory_copy(char *source, char *dest, int nbytes);

char *int_to_ascii(uint32_t val, char *buf);

#endif //TOS_UTIL_H
