#ifndef TOS_LIBC_STRING_H
#define TOS_LIBC_STRING_H
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void *s1, const void *s2, size_t n);
void *memset(void *ptr, int c, size_t n);
void *memcpy(void *__restrict dest, const void *__restrict src, size_t n);

size_t strlen(const char *str);
int strcmp(const char *str1, const char *str2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strcpy(char *__restrict dest, const char *__restrict src);
char *strncpy(char *__restrict dest, const char *__restrict src, size_t n);
char *strcat(char *destination, const char *source);
char *strncat(char *destination, const char *source, size_t n);

#ifdef __cplusplus
}
#endif

#endif //TOS_LIBC_STRING_H
