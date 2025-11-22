KERNEL_VIRT_BASE = 0xC0000000;
KERNEL_PHYS_BASE = 0x00100000;

PHDRS {
    phys PT_LOAD FLAGS(0x4);
    text PT_LOAD FLAGS(0x5);
    data PT_LOAD FLAGS(0x6);
}

SECTIONS
{
    . = KERNEL_VIRT_BASE;

    kernel_start = .;

    .text : AT(ADDR(.text) - KERNEL_VIRT_BASE + KERNEL_PHYS_BASE)
    {
        *(.text*)
    } :text

    .rodata : AT(ADDR(.rodata) - KERNEL_VIRT_BASE + KERNEL_PHYS_BASE)
    {
        *(.rodata*)
    } :text

    .data : AT(ADDR(.data) - KERNEL_VIRT_BASE + KERNEL_PHYS_BASE)
    {
        *(.data*)
    } :data

    .bss : AT(ADDR(.bss) - KERNEL_VIRT_BASE + KERNEL_PHYS_BASE)
    {
        *(.bss*)
        *(COMMON)
    } :data

    .pagetables ALIGN(4096) : AT(0x00300000)
    {
        *(.pagetables)
    } :phys

    kernel_end = .;
}
