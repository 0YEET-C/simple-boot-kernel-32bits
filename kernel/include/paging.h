#ifndef PAGEING_H
#define PAGEING_h

#include <stdint.h>

#define PAGE_PRESENT 0x1
#define PAGE_RW 0x2
#define PAGE_USER 0x3

void init_paging();
void map_page(uint32_t va, uint32_t pa, uint32_t flags);
#endif