#include "include/paging.h"
#include <stdint.h>
#include "include/string.h"
#include "include/vga.h"

#define PAGE_SIZE 4096
#define PAGE_ENTRIES 1024

#define KERNEL_BASE 0xC0000000
#define KERNEL_PHYS 0x00300000

#define STACK_TOP 0xC0600000
#define STACK_SIZE 0x4000

#define VGA_BASE 0xC00B8000
#define VGA_PHYS 0x000B8000

uint32_t page_dir[PAGE_ENTRIES] __attribute__((section(".pagetables"), aligned(PAGE_SIZE)));
uint32_t high_table[PAGE_ENTRIES] __attribute__((section(".pagetables"), aligned(PAGE_SIZE)));
uint32_t high_stack_table[PAGE_ENTRIES] __attribute__((section(".pagetables"), aligned(PAGE_SIZE)));

void init_paging() {
    memset(page_dir, 0, PAGE_SIZE);
    memset(high_table, 0, PAGE_SIZE);
    memset(high_stack_table, 0, PAGE_SIZE);

    for(int i = 0; i < PAGE_ENTRIES; i++) {
        high_table[i] = (i * PAGE_SIZE + KERNEL_PHYS) | PAGE_PRESENT | PAGE_RW ;
    }
    page_dir[KERNEL_BASE >> 22] = ((uint32_t)high_table - KERNEL_BASE + KERNEL_PHYS) | PAGE_PRESENT | PAGE_RW ;

    page_dir[(STACK_TOP >> 22)] = ((uint32_t)high_stack_table - KERNEL_BASE + KERNEL_PHYS) | PAGE_PRESENT | PAGE_RW;

    for (int i = 0; i < STACK_SIZE / PAGE_SIZE; i++) {
        high_stack_table[((STACK_TOP - STACK_SIZE + i*PAGE_SIZE) >> 12) & 0x3FF] = (0x600000 + i*PAGE_SIZE) | PAGE_PRESENT | PAGE_RW;
    }

    high_table[(VGA_BASE >> 12) & 0x3FF] = (VGA_PHYS & 0xFFFFF000) | PAGE_PRESENT | PAGE_RW;
    
    print_hex((uint32_t)page_dir);
    print("\n");

    uint32_t pd_phys = (uint32_t)page_dir - KERNEL_BASE + KERNEL_PHYS;

    print_hex((uint32_t)pd_phys);

    asm volatile ("mov %0, %%cr3" :: "r"(pd_phys));
}