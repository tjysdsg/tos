#include "string.h"
#include <stdint.h>

int memcmp(const void *s1, const void *s2, size_t n) {
  const unsigned char *a = reinterpret_cast<const unsigned char *>(s1);
  const unsigned char *b = reinterpret_cast<const unsigned char *>(s2);
  for (size_t i = 0; i < n; i++) {
    if (a[i] < b[i])
      return -1;
    else if (b[i] < a[i])
      return 1;
  }
  return 0;
}

void *memset(void *ptr, int c, uint32_t n) {
  auto *p = (uint8_t *) ptr;
  for (uint32_t i = 0; i < n; ++i) {
    *p = c;
    ++p;
  }
  return ptr;
}

void *memcpy(void *__restrict dest, const void *__restrict src, size_t n) {
  asm volatile("rep movsb"
  : "+S"(src), "+D"(dest)
  : "c"(n)
  : "flags", "memory");
  return dest;
}

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

int strcmp(const char *str1, const char *str2) {
  do {
    if (*str1 != *str2) {
      return *str1 < *str2 ? -1 : 1;
    }
    ++str1;
    ++str2;
  } while (*str1 && *str2);

  return *str1 - *str2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  while (n && *s1 && (*s1 == *s2)) {
    ++s1;
    ++s2;
    --n;
  }
  if (n == 0) {
    return 0;
  } else {
    return (*(unsigned char *) s1 - *(unsigned char *) s2);
  }
}

char *strcpy(char *__restrict dest, const char *__restrict src) {
  char *ret = dest;
  while ((*dest++ = *src++));
  return ret;
}

char *strncpy(char *__restrict dest, const char *__restrict src, size_t n) {
  char *ret = dest;
  size_t i = 0;
  while (i++ < n && (*dest++ = *src++));
  return ret;
}

char *strcat(char *destination, const char *source) {
  const size_t dest_len = strlen(destination);
  const size_t src_len = strlen(source);

  for (size_t i = 0; i < src_len; ++i) {
    destination[dest_len + i] = source[i];
  }

  destination[dest_len + src_len] = '\0';
  return destination;
}

char *strncat(char *s1, const char *s2, size_t n) {
  char *os1;

  os1 = s1;
  while (*s1++);
  --s1;
  while ((*s1++ = *s2++))
    if (--n < 0) {
      *--s1 = '\0';
      break;
    }
  return (os1);
}
