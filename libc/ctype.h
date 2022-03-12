#ifndef TOS_LIBC_CTYPE_H
#define TOS_LIBC_CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#define _ASCIIRANGE(c, start, len) (((c) - (start)) < (len))
static inline int islower(int c)  { return _ASCIIRANGE(c, 0x61, 26); }
static inline int isupper(int c)  { return _ASCIIRANGE(c, 0x41, 26); }
static inline int isdigit(int c)  { return _ASCIIRANGE(c, 0x30, 10); }
static inline int isbdigit(int c) { return _ASCIIRANGE(c, 0x30,  2); }
static inline int isodigit(int c) { return _ASCIIRANGE(c, 0x30,  8); }
static inline int isxdigit(int c) { return _ASCIIRANGE(c | 32, 0x61, 6) || isdigit(c); }
static inline int isalpha(int c)  { return _ASCIIRANGE(c | 32, 0x61, 26); }
static inline int isprint(int c)  { return _ASCIIRANGE(c, 0x20, 95) || _ASCIIRANGE(c, 128, 127); }
static inline int isgraph(int c)  { return _ASCIIRANGE(c, 0x21, 94); }
static inline int isspace(int c)  { return _ASCIIRANGE(c, 0x9, 5) || c == 0x20; }
static inline int isalnum(int c)  { return isalpha(c) || isdigit(c); }
static inline int ispunct(int c)  { return !isalnum(c) && isgraph(c); }
static inline int isblank(int c)  { return c == 0x20 || c ==  0x9; }
static inline int iscntrl(int c)  { return c  < 0x20 || c == 0x7f; }
static inline int tolower(int c) { return isupper(c) ? c + 0x20 : c; }
static inline int toupper(int c) { return islower(c) ? c - 0x20 : c; }
#undef _ASCIIRANGE

#ifdef __cplusplus
}
#endif

#endif //TOS_LIBC_CTYPE_H
