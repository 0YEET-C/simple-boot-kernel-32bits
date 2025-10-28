#include <stdint.h>
#include "include/vga.h"
#include "include/keyboard.h"
#include "include/string.h"
#include "include/io.h"

void handle_command(const char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        print("\nAvailable commands:\n");
        print("  help   - Show this help message\n");
        print("  clear  - Clear the screen\n");
        print("  about  - About this OS\n");
        print("  reboot - Reboot the system");
    } else if (strcmp(cmd, "clear") == 0) {
        clear_screen();
    } else if (strcmp(cmd, "about") == 0) {
        print("\nMY LEARING OS PROJECT\nA simple 32-bit x86 kernel written in C and Assembly.");
    } else if (strcmp(cmd, "reboot") == 0) {
        print("\nRebooting...\n");
        outb(0x64, 0xFE);
        while (1) { __asm__ volatile("hlt"); }
    }  else {
        print("\nUnknown command: ");
        print(cmd);
        print("\nType 'help' for list of commands.");
    }

}