#ifndef STRING_H
#define STRING_h

#include <stdint.h>
#include <stddef.h>

static inline void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;
    while (n--) {
        *p++ = (uint8_t)c;
    }
    return s;
}   

#endif