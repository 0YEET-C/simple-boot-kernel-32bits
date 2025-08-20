#include "vga.h"
#include "isr.h"

void isr0_handler(regs_t *r) {
    print("Interrupt: ");
    print_int(r->int_no);

    for(;;);
}
