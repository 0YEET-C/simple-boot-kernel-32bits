.PHONY: all build clean run

all: clean build

build:
	nasm -f bin boot/stage1.nasm -o stage1.bin

	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c early_boot/setup.c -o setup.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c early_boot/paging.c -o paging.o
	nasm -f elf32 early_boot/high_half_entry.S -o high_half_entry.o

	ld -m elf_i386 -T linker.ld setup.o paging.o high_half_entry.o -o setup.elf -nostdlib
	objcopy -O binary setup.elf setup.bin
	truncate -s 2560 setup.bin

	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/main.c -o main.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/vga.c -o vga.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/gdt.c -o gdt.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/idt.c -o idt.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/isr.c -o isr.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/pic.c -o pic.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/irq.c -o irq.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/keyboard.c -o keyboard.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/shell.c -o shell.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/string.c -o string.o
	nasm -f elf32 kernel/gdt_flush.S -o gdt_flush.o
	nasm -f elf32 kernel/idt_flush.S -o idt_flush.o
	nasm -f elf32 kernel/isr_handler.S -o isr_handler.o
	nasm -f elf32 kernel/irq_handler.S -o irq_handler.o

	ld -m elf_i386 -T kernel_linker.ld main.o vga.o gdt.o gdt_flush.o idt.o idt_flush.o isr.o isr_handler.o pic.o irq.o irq_handler.o keyboard.o shell.o string.o -o kernel.elf -nostdlib
	objcopy -O binary kernel.elf kernel.bin
	truncate -s 10240 kernel.bin

	cat stage1.bin setup.bin kernel.bin > test.bin

run: all
	qemu-system-i386 -vga std -drive format=raw,file=test.bin 

clean:
	rm -f *.bin *.o *.elf
