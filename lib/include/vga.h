#ifndef VGA_H
#define VGA_H

#include <stdint.h>

void clear_screen(void);
void print_char(char c);
void print(const char *str);
void print_int(int num);
void print_hex(uint32_t value);
#endif