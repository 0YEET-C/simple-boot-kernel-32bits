[bits 32]
global stage2_start
extern setup_kernel
stage2_start:
    mov ax,0x10
    mov es,ax
    mov ds,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    mov esp,0x200000

    mov esi, 0x90000
    mov edi, 0x100000
    mov ecx, 20*512/4  
    rep movsd
    
    call setup_kernel