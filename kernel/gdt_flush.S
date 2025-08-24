[bits 32]
global gdt_flush
gdt_flush:
    mov eax, [esp + 4]
    lgdt [eax]
    
    jmp 0x08:.flush

.flush:
    mov ax,0x10
    mov es,ax
    mov ds,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    ret