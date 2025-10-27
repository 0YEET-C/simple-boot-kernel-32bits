#include "include/isr.h"
#include "include/vga.h"

void isr_handler(regs_t *r) {
    if (r->int_no == 0) {
        print("Interrupt: ");
        print_int(r->int_no);

        asm volatile ("hlt");
    }else {
        print("Interrupt: ");
        print_int(r->int_no);
        print("\n");
    }
}
