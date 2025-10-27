#include "include/gdt.h"
#include "include/vga.h"
#include <stdint.h>

struct gdt_entry gdt[5];
struct gdt_ptr gdtr;

void set_gdt_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].access = access;
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[num].base_high = (base >> 24) & 0xFF;
}

extern void gdt_flush(uint32_t);

void init_gdt(void) {
    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (uint32_t)gdt;

    set_gdt_gate(0, 0, 0, 0, 0);
    
    set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    set_gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    set_gdt_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    set_gdt_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    gdt_flush((uint32_t)&gdtr);
}