#include "stdio.h"

static FILE __stdin_real = {0};
static FILE __stdout_real = {1};
static FILE __stderr_real = {2};

FILE *stdin = &__stdin_real;
FILE *stdout = &__stdout_real;
FILE *stderr = &__stderr_real;

int fprintf(FILE *stream, const char *format, ...) {
  // TODO: implement fprintf
  return 0;
}

FILE *__get_stdin() { return &__stdin_real; }

FILE *__get_stdout() { return &__stdout_real; }

FILE *__get_stderr() { return &__stderr_real; }
