OUTPUT_FORMAT("elf32-i386")
ENTRY(kmain)

SECTIONS
{
    . = 0xC0000000;
    kernel_start = .;

    .text : AT(0x100000) {
        *(.text)
    }

    .rodata : AT(0x100000 + SIZEOF(.text)) {
        *(.rodata*)
    }

    .data : AT(0x100000 + SIZEOF(.text) + SIZEOF(.rodata)) {
        *(.data)
    }

    .bss : AT(0x100000 + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.data)) {
        *(.bss COMMON)
    }

    kernel_end = .;
}
