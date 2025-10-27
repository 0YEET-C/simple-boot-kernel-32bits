#include "include/irq.h"
#include "include/vga.h"
#include "include/io.h"

void irq0_handler() {
    print("0");
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

unsigned char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',   0,
    '\\','z','x','c','v','b','n','m',',','.','/',   0,
    '*', 0,' ',
};

static int shift_pressed = 0;

void keyboard_handler() {
    unsigned char scancode = inb(0x60);

    if (scancode == 0x2A || scancode == 0x36) shift_pressed = 1;
    if (scancode == 0xAA || scancode == 0xB6) shift_pressed = 0;

    if (!(scancode & 0x80) && scancode < 128) {
        char c = scancode_to_ascii[scancode];

        if (c) {
            if (shift_pressed && c >= 'a' && c <= 'z') {
                c = c - 'a' + 'A';
            }
            print_char(c);
        }
    }

    outb(0x20, 0x20);
}