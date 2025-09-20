#include "paging.h"
#include "pmm.h"
#include <stdint.h>
#include "string.h"
#include "vga.h"

#define PAGE_SIZE 4096
#define PAGE_ENTRIES 1024

static uint32_t page_dir[PAGE_ENTRIES]        __attribute__((aligned(PAGE_SIZE)));
static uint32_t first_page_table[PAGE_ENTRIES]__attribute__((aligned(PAGE_SIZE)));
static uint32_t page_table_high[PAGE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));

void map_page(uint32_t va, uint32_t pa, uint32_t flags) {
    uint32_t pd_index = (va >> 22) & 0x3FF;
    uint32_t pt_index = (va >> 12) & 0x3FF;
    uint32_t *page_table;

    if (page_dir[pd_index] & PAGE_PRESENT) {
        page_table = (uint32_t *)(page_dir[pd_index] & 0xFFFFF000);
    }else {
        page_table = (uint32_t *)alloc_page();
        memset(page_table, 0, PAGE_SIZE);
        page_dir[pd_index] = ((uint32_t)page_table) | PAGE_PRESENT | PAGE_RW;
    }

    page_table[pt_index] = (pa & 0xFFFFF000) | flags | PAGE_PRESENT;
}

static uint32_t heap_start = 0x300000;
static uint32_t heap_end = 0x300000;

void init_paging() {
    memset(page_dir, 0, PAGE_SIZE);
    memset(page_table_high, 0, PAGE_SIZE);
    memset(first_page_table, 0, PAGE_SIZE);

    for (int i = 0; i < PAGE_ENTRIES; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW ;
    }
    page_dir[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_RW;

    for (int i = 0; i < PAGE_ENTRIES; i++) {
        page_table_high[i] = (0x00100000 + i*PAGE_SIZE)| PAGE_PRESENT | PAGE_RW ;
    }
    page_dir[0x300] = ((uint32_t)page_table_high) | PAGE_PRESENT | PAGE_RW ;

    page_table_high[0xB8] = 0x000B8000 | PAGE_PRESENT | PAGE_RW;
    
    heap_start = heap_end;

    asm volatile ("mov %0, %%cr3" :: "r"(page_dir));
    uint32_t cr0;
    asm volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile ("mov %0, %%cr0" :: "r"(cr0));
}