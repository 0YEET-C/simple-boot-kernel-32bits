[bits 32]
global entry
entry:

    mov eax,0xb8000
    mov byte [eax], 'A'
    mov byte [eax+1], 0x0F
    
    jmp 0xC0000000
