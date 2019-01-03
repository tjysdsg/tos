#ifndef TYPES_H
#define TYPES_H
// int
typedef signed int __attribute__((__mode__(QI))) int8_t;
typedef unsigned int __attribute__((__mode__(QI))) uint8_t;
typedef signed int __attribute__((__mode__(HI))) int16_t;
typedef unsigned int __attribute__((__mode__(HI))) uint16_t;
typedef signed int __attribute__((__mode__(SI))) int32_t;
typedef unsigned int __attribute__((__mode__(SI))) uint32_t;
typedef signed int __attribute__((__mode__(DI))) int64_t;
typedef unsigned int __attribute__((__mode__(DI))) uint64_t;
// typedef signed int __attribute__((__mode__(TI))) int128_t;
// typedef unsigned int __attribute__((__mode__(TI))) uint128_t;

// float
typedef float __attribute__((__mode__(SF))) float32_t;
typedef float __attribute__((__mode__(DF))) float64_t;
typedef float __attribute__((__mode__(TF))) float128_t;

// other
typedef unsigned int __attribute__((__mode__(QI))) byte_t;
typedef unsigned int __attribute__((__mode__(HI))) word_t;
typedef unsigned int __attribute__((__mode__(SI))) double_word_t;
#endif
