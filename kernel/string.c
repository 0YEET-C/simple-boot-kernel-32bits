#include "string.h"
#include <stdint.h>

#define KERNEL_HEAP_START 0x300000
#define KERNEL_HEAP_MAX   0x600000

static uint8_t* heap_end = (uint8_t*)KERNEL_HEAP_START;

void *kmalloc(uint32_t size) {
    size = (size + 3) & ~3;
    if ((uint32_t)(heap_end + size) > KERNEL_HEAP_MAX)
        return 0;
    void* ptr = heap_end;
    heap_end += size;
    return ptr;
}

void *memset(void *s, int c, size_t n) {
    uint8_t* p = (uint8_t*)s;
    while (n--) *p++ = (uint8_t)c;
    return s;
}
