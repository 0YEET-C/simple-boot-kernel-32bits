[bits 32]
global isr0
extern isr0_handler
isr0:
    cli
    push dword 0
    push dword 0
    pushad
    push ds
    push es
    push fs
    push gs

    mov eax, esp
    push eax
    call isr0_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds
    popa
    
    add esp, 8
    sti
    iretd
