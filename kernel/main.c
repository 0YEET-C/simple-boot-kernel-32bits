#include <stdint.h>
#include "vga.h"
#include "idt.h"
#include "io.h"
#include "pic.h"
#include "irq.h"
#include "gdt.h"
#include "pmm.h"


void stop_irq0() {
    uint8_t mask = inb(0x21);
    mask |= 1 << 0;
    outb(0x21, mask);
}

void start_irq0() {
    uint8_t mask = inb(0x21); 
    mask &= ~(1 << 0);
    outb(0x21, mask);
}

void kmain() {
    init_gdt();
    init_pmm();
    clear_screen();
    print("Hello form kernel\n");
    pic_remap(0x20, 0x28);
    init_idt();
    timer_init(100);
    __asm__ volatile ("sti");

    stop_irq0();

    uint32_t addr1 = alloc_page();
    print("alloc_page() = ");
    print_int(addr1);
    print("\n");

    uint32_t addr2 = alloc_page();
    print("alloc_page() = ");
    print_int(addr2);
    print("\n");

    free_page(addr1);
    print("free_page = ");
    print_int(addr1);
    print("\n");

    uint32_t addr3 = alloc_page();
    print("alloc_page() = ");
    print_int(addr3);
    print("\n");

    while(1) {}
}

