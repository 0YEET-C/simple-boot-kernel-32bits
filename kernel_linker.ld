OUTPUT_FORMAT("elf32-i386")
ENTRY(kmain)

SECTIONS
{
    . = 0xC0000000;

    .text : {
        *(.text)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss COMMON)
    }
}
