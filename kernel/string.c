#include "string.h"
#include <stdint.h>
#include "pmm.h"
#include "paging.h"

#define KERNEL_HEAP_START 0x300000
#define KERNEL_HEAP_MAX   0x700000
#define PAGE_SIZE 4096

static uint8_t* heap_end = (uint8_t*)KERNEL_HEAP_START;

void *kmalloc(uint32_t size) {
    if (size == 0) return 0;
    size = (size + 3) & ~3;

    uint8_t* old_heap_end = heap_end;
    uint8_t* new_heap_end = heap_end + size;

    if ((uint32_t)(heap_end + size) > KERNEL_HEAP_MAX) return 0;
    
    while ((uint32_t)old_heap_end / PAGE_SIZE != ((uint32_t)new_heap_end - 1) / PAGE_SIZE) {
        uint32_t page_va = ((uint32_t)old_heap_end / PAGE_SIZE) * PAGE_SIZE;
        uint32_t page_pa = alloc_page();
        if (!page_pa) return 0;
        map_page(page_va, page_pa, PAGE_RW);
        old_heap_end = (uint8_t *)(page_va + PAGE_SIZE);
    }

    heap_end = new_heap_end;
    return (void *)old_heap_end;
}

void *memset(void *s, int c, size_t n) {
    uint8_t* p = (uint8_t*)s;
    while (n--) *p++ = (uint8_t)c;
    return s;
}
