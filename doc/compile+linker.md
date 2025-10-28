# Assembly (NASM)

## Raw Binary(.bin)
```bash
nasm -f bin target.nasm -o target.bin
```
- Generates a raw binary image directly executable by the BIOS or CPU.
- Commonly used for bootloaders or stage 1 in the boot process.

## Assembly to object(.o)
```bash
nasm -f elf32 target.S -o target.o
```
- Convert Assembly to elf32 object link with C code.
- Used for kernel or low-level modules written in assembly.

# C

## C to object(.o)
```bash
i686-elf-gcc -m32 -ffreestanding -O0 -Wall -c target.c -o target.o
```
- m32 -> compile for 32-bit target.
- ffreestanding -> disable standard library dependencies.
- -O0 → disable optimizations (better for debugging).
- Wall → show all warnings.


# LINKER

# Link All Object Files
```bash
ld -m elf_i386 -T linker.ld OBJECT_INPUT.o -o ELF_OUTPUT.elf -nostdlib
```
- Combine object files into a single ELF executable using a custom linker script.
- -T linker.ld → defines memory layout and symbol placement.
- -nostdlib → omit standard startup files and libraries.

# Convert ELF to Raw Binary
```bash
objcopy -O binary ELF_OUTPUT.elf BINARY_OUTPUT.bin
```
- Converts the ELF into a flat binary image that can be loaded by the bootloader.