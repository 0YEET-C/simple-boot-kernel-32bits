[bits 16]
[org 0x7c00]
start:
    cli 
    xor ax,ax
    mov ds,ax
    mov es,ax

    mov [boot_drive], dl

    call ea_A20
    call load_stage2
    call load_kernel

    lgdt [gdt_des]
    
    cli
    mov eax,cr0
    or eax,1
    mov cr0,eax
    
    jmp 0x08:pm_mode_start

gdt_start:
    dq 0x0000000000000000
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_end:

gdt_des:
    dw gdt_end - gdt_start - 1
    dd gdt_start

ea_A20:
    in al,0x92
    or al,2
    out 0x92,al
    ret

load_stage2:
    mov dl, [boot_drive]
    lea si, [dap_stage2]
    mov ah, 0x42
    int 0x13
    jc fail
    ret

dap_stage2:
    db 0x10
    db 0
    dw 5
    dw 0
    dw 0x8000
    dq 1

load_kernel:
    mov dl, [boot_drive]
    lea si, [dap_kernel]
    mov ah, 0x42
    int 0x13
    jc fail
    ret

dap_kernel:
    db 0x10
    db 0
    dw 20
    dw 0  
    dw 0x9000 
    dq 6

fail:
    hlt
    jmp $

[bits 32]
pm_mode_start:
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov gs,ax
    mov fs,ax
    mov ss,ax
    mov esp,0x200000

    mov esi, 0x90000
    mov edi, 0x100000
    mov ecx, (20*512) / 4
    rep movsd

    jmp 0x08:0x80000

boot_drive: db 0

times 510 - ($ - $$) db 0
dw 0xAA55
