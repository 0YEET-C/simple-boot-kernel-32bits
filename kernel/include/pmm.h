#ifndef PMM_H
#define PMM_H
#include <stdint.h>

uint32_t alloc_page();
void free_page(uint32_t addr);
void init_pmm();
#endif 