#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define KEYBOARD_BUFFER_SIZE 128

typedef struct {
    char buffer[KEYBOARD_BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
} keyboard_buffer;

void keyboard_buffer_push(char c);
char keyboard_buffer_pop(void);
void keyboard_handler(void);
void shell();

#endif