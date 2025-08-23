#include <stdint.h>
#include "../lib/include/vga.h"
#include "include/idt.h"
#include "include/io.h"
#include "include/pic.h"
#include "include/irq.h"


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
    clear_screen();
    print("BOOT....\n");
    pic_remap(0x20, 0x28);
    init_idt();
    timer_init(100);
    __asm__ volatile ("sti");

    stop_irq0();
    
    while(1) {}
}

