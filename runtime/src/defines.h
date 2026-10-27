#pragma once
#include <stdint.h>
#include <stddef.h>

typedef unsigned char u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef signed char i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef u8 byte;
typedef size_t usize;

typedef float f32;
typedef double f64;

typedef char b8;
typedef int b32;
#define TRUE 1
#define FALSE 0

#define VINLINE static inline

#define VMAX(a, b) ((a) > (b) ? (a) : (b))
#define VMIN(a, b) ((a) < (b) ? (a) : (b))
#define VCLAMP(x, min, max) MAX(min, MIN(x, max))

#define VARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

#ifdef DEBUG
#define VDEBUG_BREAK() __asm__("int3")
#else
#define VDEBUG_BREAK() ((void)0)
#endif
