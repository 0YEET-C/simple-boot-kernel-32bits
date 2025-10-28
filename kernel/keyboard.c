#include <stdint.h>
#include "include/io.h"
#include "include/keyboard.h"
#include "include/vga.h"
#include "include/shell.h"

keyboard_buffer_t kbd_buffer = {0};

static unsigned char scancode_to_ascii[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,
    '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0,' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static int shift_pressed = 0;

void keyboard_buffer_push(char c) {
    uint8_t next = (kbd_buffer.head + 1) % KEYBOARD_BUFFER_SIZE;
    if (next != kbd_buffer.tail) {
        kbd_buffer.buffer[kbd_buffer.head] = c;
        kbd_buffer.head = next;
    }
}

char keyboard_buffer_pop(void) {
    if (kbd_buffer.head == kbd_buffer.tail) return 0;
    char c = kbd_buffer.buffer[kbd_buffer.tail];
    kbd_buffer.tail = (kbd_buffer.tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

void keyboard_handler(void) {
    unsigned char scancode = inb(0x60);

    if (scancode == 0x2A || scancode == 0x36) shift_pressed = 1;
    if (scancode == 0xAA || scancode == 0xB6) shift_pressed = 0;

    if (!(scancode & 0x80) && scancode < 128) {
        char c = scancode_to_ascii[scancode];
        if (c) {
            if (shift_pressed && c >= 'a' && c <= 'z')
                c = c - 'a' + 'A';
            keyboard_buffer_push(c);
        }
    }

    outb(0x20, 0x20);
}

void shell() {
    char c;
    char input[128];
    int idx = 0;

    print(">");
    while(1) {
        c = keyboard_buffer_pop();
        if(c) {
            if (c == '\n') {
                input[idx] = 0;
                handle_command(input);
                idx = 0;
                print("\n> ");
            } else if (c == '\b' && idx > 0) {
                idx--;
                print_char('\b');
                print_char(' ');
                print_char('\b');
            } else {
                input[idx++] = c;
                print_char(c);
            }
        }
    }
}