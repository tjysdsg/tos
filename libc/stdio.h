#ifndef TOS_LIBC_STDIO_H
#define TOS_LIBC_STDIO_H
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  size_t fd;
} FILE;

int fprintf(FILE *stream, const char *format, ...);

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

#ifdef __cplusplus
}
#endif

#endif //TOS_LIBC_STDIO_H