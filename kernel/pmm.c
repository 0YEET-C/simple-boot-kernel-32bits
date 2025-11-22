#include <stdint.h>
#include "include/pmm.h"
#include "include/string.h"

#define TOTAL_MEMORY (4*1024*1024)
#define TOTAL_PAGE (TOTAL_MEMORY / PAGE_SIZE)
#define BITMAP_SIZE (TOTAL_PAGE / 8)

uint8_t memory_bitmap[BITMAP_SIZE];

void set_page_used(uint32_t page) {
    memory_bitmap[page/8] |= (1 << (page % 8));
}

void set_page_free(uint32_t page) {
    memory_bitmap[page/8] &= ~(1 << (page % 8));
}

int is_page_free(uint32_t page) {
    return !(memory_bitmap[page/8] & (1 << (page % 8)));
}

uint32_t alloc_page() {
    for (uint32_t i = 0; i < TOTAL_PAGE; i++) {
        if (is_page_free(i)) {
            set_page_used(i);
            return i*PAGE_SIZE;      
        }
    }
    return 0xFFFFFFFF;
}

void free_page(uint32_t addr) {
    uint32_t page = addr/PAGE_SIZE;
    set_page_free(page);
}

extern uint32_t kernel_start;
extern uint32_t kernel_end;

void init_pmm() {
    memset(memory_bitmap, 0, sizeof(memory_bitmap));
    
    set_page_used(0);

    uint32_t start = (uint32_t)&kernel_start;
    uint32_t end = (uint32_t)&kernel_end;

    uint32_t start_page = start/PAGE_SIZE;
    uint32_t end_page = (end + PAGE_SIZE - 1)/PAGE_SIZE;

    for (uint32_t i = start_page; i < end_page; i++) {
        set_page_used(i);
    }
}

