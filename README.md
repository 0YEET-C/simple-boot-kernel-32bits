# MyOS

**MyOS** is a simple 32-bit kernel developed for learning and portfolio purposes.  
It runs on x86 in **protected mode** and demonstrates basic OS concepts.

# FOR FULL EXPLAIN IN DOC FILE

## OS Concepts Implemented
- Bootloader
- Interrupt Descriptor Table (IDT)
- Interrupt Service Routine (ISR)
- Interrupt Request (IRQ)
- Timer & Keyboard Interrupts

## Features
- Clean and modular code
- Thread
- IPC
- IRQ dispatcher
- Memory allocator
- System calls & User mode (planned)

## Toolchain & Compilation
MyOS is **cross-compiled** from scratch using:
- **NASM** for assembly files
- **GCC** (elf32-i386 target) for C files
- **LD** for linking
- **Binutils** for assembling and linking utilities

All components are compiled manually, without relying on prebuilt kernels.

### Build Steps
for full compiled in Makefile
```bash
# Assemble bootloader
nasm -f bin boot/stage1.nasm -o stage1.bin

# Assemble 32-bit loader
nasm -f elf32 boot/stage2.nasm -o stage2.o

# Compile C kernel
i686-elf-gcc -ffreestanding -m32 -O0 -c kernel/main.c -o main.o

# Link kernel
i686-elf-ld -T linker.ld -nostdlib -o stage2.elf stage2.o main.o
i686-elf-objcopy -O binary stage2.elf stage2.bin
truncate -s 10240 stage2.bin

# Combine with bootloader into raw binary
cat stage1.bin stage2.bin > test.bin
```

# Run with QEMU
qemu-system-i386 -drive format=raw,file=test.bin
