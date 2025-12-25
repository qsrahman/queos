#pragma once

#include <stdint.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

// Rounding operations (efficient when n is a power of 2)
// Round down to the nearest multiple of n
#define ROUNDDOWN(a, n)                   \
    ({                                    \
        uint32_t __a = (uint32_t)(a);     \
        (__typeof__(a))(__a - __a % (n)); \
    })
// Round up to the nearest multiple of n
#define ROUNDUP(a, n)                                             \
    ({                                                            \
        uint32_t __n = (uint32_t)(n);                             \
        (__typeof__(a))(ROUNDDOWN((uint32_t)(a) + __n - 1, __n)); \
    })

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

void panic(const char* message, const char* file, int line);
void panic_assert(const char* file, int line, const char* desc);
