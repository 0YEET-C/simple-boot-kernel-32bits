#include "../lib/include/vga.h"
#include "include/isr.h"

void isr_handler(regs_t *r) {
    print("Interrupt: ");
    print_int(r->int_no);
    print("\n");

}
