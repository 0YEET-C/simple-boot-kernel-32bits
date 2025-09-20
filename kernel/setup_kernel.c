#include <stdint.h>
#include "vga.h"
#include "idt.h"
#include "io.h"
#include "pic.h"
#include "irq.h"
#include "gdt.h"
#include "pmm.h"
#include "string.h"
#include "paging.h"
#include "thread.h"

// typedef void (*kentry_t)(void);

void setup_kernel() {
    init_paging();
    clear_screen();
    init_gdt();
    init_pmm();
    init_idt();
    pic_remap(0x20, 0x28);

    timer_init();
    __asm__ volatile ("sti");

    print("SETUP....\n");

    map_page(0xC0000000, 0x00000000, PAGE_PRESENT | PAGE_RW);

    uint32_t *addr = (uint32_t*)0xC0000000;
    addr[0] = 0xDEADBEEF;
    print_hex(*(uint32_t*)0x00000000);
    


    // uint32_t kernel_stack = 0xC03FF000;
    
    // __asm__ volatile ("mov %0, %%esp\n"
    //                   "xor %%ebp, %%ebp\n"
    //                   :: "r"(kernel_stack) : "memory");

    // kentry_t kernel = (kentry_t)0xC0000000;
    // kernel;

    while(1) {}
}

