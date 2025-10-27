#include <stdint.h>
#include "include/paging.h"

#define PAGE_SIZE 4096
#define PAGE_ENTRIES 1024

#define KERNEL_BASE 0xC0000000
#define KERNEL_PHYS 0x00100000

#define STACK_TOP 0xC0700000
#define STACK_SIZE 0x4000

#define VGA_BASE 0xC00B8000
#define VGA_PHYS 0x000B8000

static uint32_t page_dir[PAGE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
static uint32_t low_table[PAGE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
static uint32_t high_table[PAGE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
static uint32_t high_stack_table[PAGE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));

void init_paging() {

    for (int i = 0; i < PAGE_ENTRIES; i++) {
        page_dir[i] = 0;
        low_table[i] = 0;
        high_table[i] = 0;
    }
    
    for (int i = 0; i < PAGE_ENTRIES; i++) {
        low_table[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW ;
        high_table[i] = (i * PAGE_SIZE + KERNEL_PHYS) | PAGE_PRESENT | PAGE_RW ;
    }   
    page_dir[0] = ((uint32_t)low_table) | PAGE_PRESENT | PAGE_RW ;
    page_dir[KERNEL_BASE >> 22] = ((uint32_t)high_table) | PAGE_PRESENT | PAGE_RW ;

    page_dir[(STACK_TOP >> 22)] = ((uint32_t)high_stack_table) | PAGE_PRESENT | PAGE_RW;

    for (int i = 0; i < STACK_SIZE / PAGE_SIZE; i++) {
        high_stack_table[((STACK_TOP - STACK_SIZE + i*PAGE_SIZE) >> 12) & 0x3FF] = (0x700000 + i*PAGE_SIZE) | PAGE_PRESENT | PAGE_RW;
    }

    high_table[(VGA_BASE >> 12) & 0x3FF] = (VGA_PHYS & 0xFFFFF000) | PAGE_PRESENT | PAGE_RW;


    asm volatile ("mov %0, %%cr3" :: "r"(page_dir));
    uint32_t cr0;
    asm volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile ("mov %0, %%cr0" :: "r"(cr0));

    asm volatile ("mov %0, %%esp" :: "r"(STACK_TOP));
}

#define BUFFER 0x90000
#define BUFFER_SIZE   (20*512)

void clear_buffer() {
    uint32_t *ptr = (uint32_t *)BUFFER;
    for(int i = 0; i < BUFFER_SIZE/4; i++) {
        ptr[i] = 0;
    }
}