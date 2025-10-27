#include <stdint.h>
#include "include/vga.h"

#define VGA_WIDTH 80
#define VGA_HIGHT 25

volatile uint16_t *vga = (volatile uint16_t*)0xc00b8000;
static int row = 0, col = 0;

void clear_screen() {
    for (int i = 0; i < VGA_WIDTH*VGA_HIGHT; i++) {
        vga[i] = (0x07 << 8) | ' ';
    }
    row = 0;
    col = 0;
}

void print_char(char c) {
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

static const char hex_digits[] = "0123456789ABCDEF";

void print_hex(uint32_t value) {
    print("0x");

    for (int i = 28; i >= 0; i -= 4) {
        uint8_t nibble = (value >> i) & 0xF;
        print_char(hex_digits[nibble]);
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
