#ifndef PMM_H
#define PMM_H

#define PAGE_SIZE 4096

#define HIGH_HALF 0xC0000000

#include <stdint.h>


uint32_t alloc_page();
void free_page(uint32_t addr);
void init_pmm();
#endif