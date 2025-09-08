#include "paging.h"
#include "pmm.h"
#include <stdint.h>
#include "string.h"
#include "vga.h"

#define PAGE_SIZE 4096
#define PAGE_ENTRIES 1024

static uint32_t *page_dir __attribute__((aligned(PAGE_SIZE)));
static uint32_t *first_page_table __attribute__((aligned(PAGE_SIZE)));

void map_page(uint32_t va, uint32_t pa, uint32_t flags) {
    uint32_t pd_index = (va >> 22) & 0x3FF;
    uint32_t pt_index = (va >> 12) & 0x3FF;

    uint32_t *page_table;

    if (page_dir[pd_index] & PAGE_PRESENT) {
        page_table = (uint32_t *)(page_dir[pd_index] & 0xFFFFF000);
    }else {
        page_table = (uint32_t *)alloc_page();
        memset(page_table, 0, PAGE_SIZE);
        page_dir[pd_index] = ((uint32_t)page_table) | flags | PAGE_PRESENT;
    }

    page_table[pt_index] = (pa & 0xFFFFF000) | flags | PAGE_PRESENT;
}

void init_paging() {
    page_dir = (uint32_t *)alloc_page();
    memset(page_dir, 0, PAGE_SIZE);

    first_page_table = (uint32_t *)alloc_page();
    memset(first_page_table, 0, PAGE_SIZE);

    for (int i = 0; i< PAGE_ENTRIES; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW; 
    }

    page_dir[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_RW;

    asm volatile ("mov %0, %%cr3" :: "r"(page_dir));

    uint32_t cr0;
    asm volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile ("mov %0, %%cr0" :: "r"(cr0));
}