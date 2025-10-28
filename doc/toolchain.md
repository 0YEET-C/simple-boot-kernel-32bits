# Build Requirements
### - NASM — Assembler for x86
### - i686-elf-gcc — Cross compiler for 32-bit systems
### - LD — GNU linker
### - objcopy — Convert ELF → raw binary
### - QEMU — Emulator for testing

## Install example

### Ubuntu/Debian
```bash 
sudo apt install nasm qemu
```

### Windows (using MSYS2 or WSL)

#### Option 1: MSYS2
- Install MSYS2(https://www.msys2.org/)
- Open MSYS2 MinGW32 termina
- run
```bash
pacman -Syu
pacman -S mingw-w64-i686-gcc nasm make qemu
```
- To use a real cross-compiler (freestanding):
```bash
git clone https://github.com/lordmilko/i686-elf-tools.git
export PATH=$PATH:/c/path/to/i686-elf-tools/bin
```
#### Option 2: WSL (Windows Subsystem for Linux)
- Install Ubuntu from Microsoft Store
- Inside WSL:
```bash 
sudo apt update
sudo apt install nasm qemu build-essential
git clone https://github.com/lordmilko/i686-elf-tools.git
export PATH=$PATH:/home/yourname/i686-elf-tools/bin
```
- You can now build using i686-elf-gcc, ld, objcopy, etc.

### macOS
- Install Homebrew(https://brew.sh/)
- Run:
```bash
brew install nasm qemu
```
- Build or install cross-compiler:
```bash
git clone https://github.com/lordmilko/i686-elf-tools.git
export PATH=$PATH:/Users/yourname/i686-elf-tools/bin
```
## Verify Installation
```bash
nasm -v
i686-elf-gcc --version
ld --version
qemu-system-i386 --version
```
