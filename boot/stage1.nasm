[bits 16]
[org 0x7c00]
start:
    cli          ; Disable interrupts
    xor ax,ax    ; Clear register
    mov ds,ax
    mov es,ax

    mov [boot_drive], dl

    call ea_A20
    call load_kernel

    lgdt [gdt_des]      ; Load the GDT descriptor
    
    cli
    mov eax,cr0
    or eax,1             ; Set PE bit → enable Protected Mode
    mov cr0,eax
    
    jmp 0x08:pm_mode_start  ; Far jmp to PM mode

gdt_start:
    dq 0x0000000000000000   ; Null descriptor
    dq 0x00CF9A000000FFFF   ; Code segment descriptor (0x08)
    dq 0x00CF92000000FFFF   ; Data segment descriptor (0x08)
gdt_end:

gdt_des:
    dw gdt_end - gdt_start - 1
    dd gdt_start

ea_A20:
    in al,0x92
    or al,2
    out 0x92,al
    ret

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
    dq 1

fail:
    hlt
    jmp $

[bits 32]
pm_mode_start:
    mov ax,0x10            ;Data segment selector
    mov ds,ax
    mov es,ax
    mov gs,ax
    mov fs,ax
    mov ss,ax
    mov esp,0x200000        ;Set stack pointer to 2 MB

    ; Copy kernel from 0x90000 to 0x100000
    mov esi, 0x90000
    mov edi, 0x100000
    mov ecx, (20*512) / 4
    rep movsd
                            
    jmp 0x08:0x100000        ; Jump to early_boot entry point

boot_drive: db 0

times 510 - ($ - $$) db 0   ; Fill the remaining bytes until offset 510
dw 0xAA55                   ; Boot sector signature