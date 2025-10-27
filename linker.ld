OUTPUT_FORMAT("elf32-i386")
ENTRY(setup)

SECTIONS
{
    . = 0x80000;
    kernel_start = .;

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

    kernel_end = .;
}
