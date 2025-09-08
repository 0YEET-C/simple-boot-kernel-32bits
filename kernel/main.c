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

void kmain() {
    clear_screen();
    init_gdt();
    init_pmm();
    init_idt();
    init_paging();
    print("Hello form kernel\n");
    pic_remap(0x20, 0x28);

    timer_init();
    __asm__ volatile ("sti");

    uint32_t *arr0 = (uint32_t *)alloc_page();
    uint32_t *arr1 = (uint32_t *)alloc_page();
    
    *arr0 = 0x123456;
    map_page((uint32_t)arr1, (uint32_t)arr0, PAGE_RW);
    print_hex((uint32_t)*arr0);
    while(1) {}
}

