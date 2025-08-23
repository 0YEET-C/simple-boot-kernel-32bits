#include "include/irq.h"
#include "../lib/include/vga.h"
#include "include/io.h"

static uint32_t timer_frequency = 18;

void irq_handler(uint32_t int_no) {
    if (int_no == 32) {
        static uint32_t ticks = 0;
        ticks++;

        if(ticks % timer_frequency == 0) {
            print("1 second passed\n");
        }
    }

    if (int_no != 32) {
        print("IRQ: ");
        print_int(int_no);
        print("\n");
    }

    if (int_no >= 40) {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
}

void timer_init(uint32_t frequency) {
    timer_frequency = frequency;

    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}


unsigned char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',   0,
    '\\','z','x','c','v','b','n','m',',','.','/',   0,
    '*', 0,' ',
};

void keyboard_handler() {
    unsigned char scancode = inb(0x60);

    if (!(scancode & 0x80) && scancode < 128) {
        char c = scancode_to_ascii[scancode];

        if (c) {
            print_char(c);
        }
    }

    outb(0x20, 0x20);
}