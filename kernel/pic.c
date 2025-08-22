#include "include/pic.h"
#include <stdint.h>
#include "include/io.h"

void pic_remap(int offset1, int offset2) {
    uint8_t master, slave;

    master = inb(0x21);
    slave = inb(0xA1);

    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, offset1);
    outb(0xA1, offset2);

    outb(0x21, 0x02);
    outb(0xA1, 0x04);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, master);
    outb(0xA1, slave);
}
