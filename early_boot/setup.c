#include <stdint.h>
#include "include/paging.h"

extern void entry();

void setup() {
    clear_buffer();
    init_paging();

    entry();
    while(1);
}
