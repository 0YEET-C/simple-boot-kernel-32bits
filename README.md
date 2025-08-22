MyOS is a simple 32-bit kernel developed for learning and portfolio purposes.
It runs on x86 in protected mode and demonstrates basic OS concepts:
- Bootloader
- Interrupt Descriptor Table (IDT)
- Interrupt Service Routine(ISR)
- Interrupt Request(IRQ)
- Timer & Keyboard Interrupts

## Features
- Clean code
- IRQ dispatcher
- Memory allocator
- System calls & User mode
 
## Build & Run
I use elf32-i386 for taget and corss-compile use binutils for toolchain 
use ld for linker use gcc for c and nasm for asm file 
I compile by myself form beginning  
run ues qemu-system-i386 -drive format=raw,file=test.bin or some ex in my Repositories 
