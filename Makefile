all: clean
	nasm -f bin boot/stage1.nasm -o stage1.bin
	nasm -f elf32 boot/stage2.nasm -o stage2.o

	i686-elf-gcc -ffreestanding -m32 -O0 -Wall -Wextra -c kernel/main.c -o main.o
	i686-elf-gcc -ffreestanding -m32 -O0 -Wall -Wextra -c lib/vga.c -o vga.o
	i686-elf-gcc -ffreestanding -m32 -O0 -Wall -Wextra -c kernel/idt.c -o idt.o
	i686-elf-gcc -ffreestanding -m32 -O0 -Wall -Wextra -c kernel/isr.c -o isr.o
	i686-elf-gcc -ffreestanding -m32 -O0 -Wall -Wextra -c kernel/pic.c -o pic.o
	i686-elf-gcc -ffreestanding -m32 -O0 -Wall -Wextra -c kernel/irq.c -o irq.o
	i686-elf-gcc -ffreestanding -m32 -O0 -Wall -Wextra -c kernel/gdt.c -o gdt.o
	nasm -f elf32 kernel/idt_flush.S -o idt_flush.o
	nasm -f elf32 kernel/isr_handler.S -o isr_handler.o
	nasm -f elf32 kernel/irq_handler.S -o irq_handler.o
	nasm -f elf32 kernel/gdt_flush.S -o gdt_flush.o

	i686-elf-ld -T linker.ld -nostdlib -o stage2.elf stage2.o main.o vga.o idt.o idt_flush.o isr.o isr_handler.o irq.o irq_handler.o pic.o gdt.o gdt_flush.o
	i686-elf-objcopy -O binary stage2.elf stage2.bin
	truncate -s 10240 stage2.bin

	cat stage1.bin stage2.bin > test.bin

run: all
	qemu-system-i386 -drive format=raw,file=test.bin -serial stdio

clean:
	rm -f *.bin *.o *.elf
