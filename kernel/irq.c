#include "include/irq.h"
#include "../lib/include/vga.h"
#include "include/io.h"

void irq_handler(uint32_t int_no) {
    print("IRQ: ");
    print_int(int_no);
    print("\n");

    if (int_no >= 40) {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
}