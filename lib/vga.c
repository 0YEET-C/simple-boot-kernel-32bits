#include <stdint.h>
#include "include/vga.h"

static int row = 0, col = 0;

void clear_screen() {
    volatile uint8_t *vga = (volatile uint8_t*)0xb8000;
    for (int i = 0; i < 80*25; i++) {
        vga[i*2] = ' ';
        vga[i*2+1] = 0x07;
    }
    row = 0;
    col = 0;
}

void print_char(char c) {
    volatile uint16_t* vga = (volatile uint16_t*)0xB8000;

    if (c == '\n') {
        col = 0;
        row++;
    }else if(c == '\b') {
        if(col > 0) {
            col--;
            vga[row*80+col] = (0x0F << 8) | ' ';
        }
    }else {
        vga[row*80+col] = (0x0F << 8) | c;
        col++;
    }

    if (col >= 80) {col = 0; row++;}
    if (row >= 25) {row = 0; clear_screen();}
}

void print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_char(str[i]);
    }
}


void itoa(int value, char* buffer) {
    char temp[12];
    int i = 0;
    int isNegative = 0;

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    if (value < 0) {
        isNegative = 1;
        value = -value;
    }

    while (value != 0) {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    }

    if (isNegative) temp[i++] = '-';

    int j = 0;
    while (i > 0) buffer[j++] = temp[--i];
    buffer[j] = '\0';
}

void print_int(int num) {
    char buffer[12];
    itoa(num, buffer);
    print(buffer);
}
