.PHONY: build bootloader kernel build clean run

build: clean bootloader kernel

bootloader:
	nasm -f bin boot/stage1.nasm -o stage1.bin


kernel:
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/main.c -o main.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/paging.c -o paging.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/vga.c -o vga.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/gdt.c -o gdt.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/idt.c -o idt.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/isr.c -o isr.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/pic.c -o pic.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/irq.c -o irq.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/keyboard.c -o keyboard.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/shell.c -o shell.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/string.c -o string.o
	i686-elf-gcc -m32 -ffreestanding -O0 -Wall -g -c kernel/pmm.c -o pmm.o
	nasm -f elf32 kernel/gdt_flush.S -o gdt_flush.o
	nasm -f elf32 kernel/idt_flush.S -o idt_flush.o
	nasm -f elf32 kernel/isr_handler.S -o isr_handler.o
	nasm -f elf32 kernel/irq_handler.S -o irq_handler.o

	ld -m elf_i386 -T linker.ld main.o paging.o vga.o gdt.o gdt_flush.o idt.o idt_flush.o isr.o isr_handler.o pic.o irq.o irq_handler.o keyboard.o shell.o string.o pmm.o -o kernel.elf -nostdlib
	objcopy -O binary kernel.elf kernel.bin
	truncate -s 10240 kernel.bin

	cat stage1.bin kernel.bin > test.bin

run: build
	qemu-system-i386 -vga std -drive format=raw,file=test.bin 

clean:
	rm -f *.bin *.o *.elf
