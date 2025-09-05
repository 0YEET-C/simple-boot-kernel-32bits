#include <stdint.h>
#include "vga.h"
#include "idt.h"
#include "io.h"
#include "pic.h"
#include "irq.h"
#include "gdt.h"
#include "pmm.h"
#include "string.h"

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
    init_gdt();
    init_pmm();
    clear_screen();
    print("Hello form kernel\n");
    pic_remap(0x20, 0x28);
    init_idt();
    timer_init(100);
    __asm__ volatile ("sti");

    stop_irq0();

    int* arr = (int*)kmalloc(5 * sizeof(int));
    for(int i=0;i<5;i++) arr[i] = i*10;

    print("array = ");
    for(int i=0;i<5;i++) {
        print_int(arr[i]);
        print(" ");
    }
    print("\n");


    while(1) {}
}

