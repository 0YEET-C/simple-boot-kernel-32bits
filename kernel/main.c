#include <stdint.h>
#include "vga.h"
#include "idt.h"

void kmain() {
    clear_screen();
    print("BOOT....\n");
    init_idt();
    print("\n");

    asm volatile("int $0");

    while(1) {}
}