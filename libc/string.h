#ifndef TOS_LIBC_STRING_H
#define TOS_LIBC_STRING_H
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int strncmp(const char *s1, const char *s2, size_t n);

#ifdef __cplusplus
}
#endif

#endif //TOS_LIBC_STRING_H
