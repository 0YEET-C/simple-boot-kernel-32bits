#include <stdint.h>
#include "vga.h"
#include "idt.h"

void kmain() {
    clear_screen();
    print("BOOT....\n");
    init_idt();
    print_int(132);
    print("\n");

    int x = 1/0;
    (void)x;

    while(1) {}
}