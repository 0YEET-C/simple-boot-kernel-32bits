#include <stdint.h>
#include "../lib/include/vga.h"
#include "include/idt.h"
#include "include/io.h"
#include "include/pic.h"

void kmain() {
    clear_screen();
    print("BOOT....\n");
    pic_remap(0x20, 0x28);
    init_idt();
    print("\n");
    while(1) {}
}

