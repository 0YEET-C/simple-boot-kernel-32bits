[bits 32]
global stage2_start
extern kmain
stage2_start:
    mov ax,0x10
    mov es,ax
    mov ds,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    mov esp,0x200000

    call kmain