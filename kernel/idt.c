#include "include/idt.h"
#include <stdint.h>

struct idt_entry idt[256];
struct idt_ptr idtp;

void set_idt_gate(uint8_t num, uint32_t handler, uint16_t sel, uint8_t flags) {
    idt[num].offset_low = handler & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
    idt[num].offset_high = (handler >> 16) & 0xFFFF;
}

extern void idt_flush(uint32_t);
extern uint32_t (*isr_table[32]);
extern uint32_t (*irq_table[16]);


void init_idt(void) {
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)idt;

    for (int i = 0; i < 256; i++) {
        set_idt_gate(i, 0, 0x08, 0x8E);
    }

    for (int v = 0; v < 32; v++) {
        set_idt_gate(v, (uint32_t)isr_table[v], 0x08, 0x8E);
    }

    for (int q = 0; q < 16; q++) {
        set_idt_gate(32 + q, (uint32_t)irq_table[q], 0x08, 0x8E);
    }
    idt_flush((uint32_t)&idtp);
}