#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

void *kmalloc(uint32_t size);
void *memset(void *s, int c, size_t n);

#endif
