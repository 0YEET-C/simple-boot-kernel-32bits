#include "include/irq.h"
#include "include/vga.h"
#include "include/io.h"

void irq0_handler() {
    outb(0x20, 0x20);
}

void timer_init() {
    uint32_t frequency = 100;

    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}

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

void irq_handler(uint32_t int_no) {
    print("IRQ: ");
    print_int(int_no);
    print("\n");

    if (int_no >= 40) {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
}
