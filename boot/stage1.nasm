[org 0x7c00]
[bits 16]
start:
    cli
    xor ax,ax
    mov es,ax
    mov ds,ax

    call ea_A20

    mov ax,0x8000
    mov es,ax
    xor bx,bx

    mov ah,0x02
    mov al,4
    mov ch,0
    mov cl,2
    mov dh,0
    mov dl,0x80
    int 0x13
    jc fail

    lgdt [gdt_des]

    mov eax,cr0
    or eax,1
    mov cr0,eax

    jmp 0x08:pm_mode_start

fail:
    hlt
    jmp $

ea_A20:
    in al,0x92
    or al,2
    out 0x92,al
    ret

gdt_start:
    dq 0x0000000000000000
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_end:

gdt_des:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[bits 32]
pm_mode_start:
    mov ax,0x10
    mov es,ax
    mov ds,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    mov esp,0x9FF00

    jmp 0x08:0x80000

times 510 - ($ - $$) db 0
dw 0xAA55