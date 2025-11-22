#ifndef EARLY_PAGING_H
#define EARLY_PAGING_H

#define PAGE_PRESENT 0x1
#define PAGE_RW 0x2
#define PAGE_USER 0x4

void clear_buffer();
void init_paging();
#endif