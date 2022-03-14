#ifndef TOS_LIBC_CTYPE_H
#define TOS_LIBC_CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

// https://github.com/bminor/musl/blob/master/src/ctype

static inline int islower(int c) { return (unsigned) c - 'a' < 26; }
static inline int isupper(int c) { return (unsigned) c - 'A' < 26; }
static inline int isdigit(int c) { return (unsigned) c - '0' < 10; }
static inline int isxdigit(int c) { return isdigit(c) || ((unsigned) c | 32) - 'a' < 6; }
static inline int isalpha(int c) { return ((unsigned) c | 32) - 'a' < 26; }
static inline int isprint(int c) { return (unsigned) c - 0x20 < 0x5f; }
static inline int isgraph(int c) { return (unsigned) c - 0x21 < 0x5e; }
static inline int isspace(int c) { return c == ' ' || (unsigned) c - '\t' < 5; }
static inline int isalnum(int c) { return isalpha(c) || isdigit(c); }
static inline int ispunct(int c) { return !isalnum(c) && isgraph(c); }
static inline int tolower(int c) { return isupper(c) ? c | 32 : c; }
static inline int toupper(int c) { return islower(c) ? c & 0x5f : c; }

#ifdef __cplusplus
}
#endif

#endif //TOS_LIBC_CTYPE_H
