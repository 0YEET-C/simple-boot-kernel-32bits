#include <stdint.h>
#include "include/vga.h"
#include "include/idt.h"
#include "include/io.h"
#include "include/pic.h"
#include "include/irq.h"
#include "include/gdt.h"
#include "include/shell.h"
#include "include/keyboard.h"
#include "include/pmm.h"
#include "include/paging.h"

void kmain() {
    init_paging();
    clear_screen();
    init_pmm();
    init_gdt();
    init_idt();
    pic_remap(0x20, 0x28);

    timer_init();
    __asm__ volatile ("sti");

    print("SETUP....\n");
    shell();
    while(1) {}
}

