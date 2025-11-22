#ifndef SHELL_H
#define SHEL_H

typedef struct {
    char buffer [16][128];
    int op_count;
} command_buffer;

void handle_command(const char *cmd);
#endif