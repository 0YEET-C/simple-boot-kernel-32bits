#ifndef STRING_H
#define STRING_H

#include <stddef.h>
void *memset(void *s, int c, size_t n);
int strcmp(const char *a, const char *b);
void *memcpy(void *dest, const void *src, size_t n);
void strcpy(char *dest, const char *src);
#endif