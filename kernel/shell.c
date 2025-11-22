#include <stdint.h>
#include "include/vga.h"
#include "include/keyboard.h"
#include "include/string.h"
#include "include/io.h"
#include "include/shell.h"

command_buffer buffer(const char *cmd) {
    command_buffer comb = {0};
    char buf[128];
    int cmd_idx = 0, buf_idx = 0;

    while (cmd[cmd_idx] != '\0') {

        if (cmd[cmd_idx] == ' ') {
            if (buf_idx > 0) {
                buf[buf_idx] = '\0';
                strcpy(comb.buffer[comb.op_count], buf);
                comb.op_count++;
                buf_idx = 0;
            }
            cmd_idx++;
            continue;
        }
        buf[buf_idx++] = cmd[cmd_idx++];
    }

    if (buf_idx > 0) {
        buf[buf_idx] = '\0';
        strcpy(comb.buffer[comb.op_count], buf);
        comb.op_count++;
    }

    return comb;
}

void handle_command(const char *cmd) {
    command_buffer comb = buffer(cmd);

    if (comb.op_count == 0) return;

    const char *cmd_0 = comb.buffer[0];

    if (strcmp(cmd_0, "help") == 0) {
        print("\nAvailable commands:\n");
        print("  help   - Show this help message\n");
        print("  echo   - Print message");
        print("  clear  - Clear the screen\n");
        print("  about  - About this OS\n");
        print("  reboot - Reboot the system");
    } else if (strcmp(cmd_0, "clear") == 0) {
        clear_screen();
    } else if (strcmp(cmd_0, "about") == 0) {
        print("\nMY LEARING OS PROJECT\nA simple 32-bit x86 kernel written in C and Assembly.");
    } else if (strcmp(cmd_0, "reboot") == 0) {
        print("\nRebooting...\n");
        outb(0x64, 0xFE);
        while (1) { __asm__ volatile("hlt"); }
    } else if (strcmp(cmd_0, "echo") == 0) {
        if (comb.op_count > 0) {
            print("\n");
            print(comb.buffer[1]);
        } 
    } else {
        print("\nUnknown command: ");
        print(cmd_0);
        print("\nType 'help' for list of commands.");
    }

}