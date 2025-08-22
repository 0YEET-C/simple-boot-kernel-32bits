# flow
Real Mode → Enable A20 → Load Kernel → Setup GDT → Protected Mode → Jump to Stage2

# BOOT STSGE1
- start at 16bits at 0x7c00
```bash
cli
xor ax, ax
mov es, ax
mov ds, ax
```
- Disable interrupts(cli)to safely configure CPU
- Initialize segment registers (ds, es) to 0

## Enable A20 line
```bash
ea_A20:
    in al,0x92
    or al,2
    out 0x92,al
    ret
```
- use port 0x92 for easy to Enable A20
- A20 line enables memory >1MB
- for Enable use some 32bits regis

## Load Stage2 from Disk
```bash
    mov ax,0x8000
    mov es,ax
    xor bx,bx

    mov ah,0x02
    mov al,20
    mov ch,0
    mov cl,2
    mov dh,0
    mov dl,0x80
    int 0x13
    jc fail
```
- Uses BIOS interrupt int 0x13 to read sectors 
- Uses BIOS interrupt 0x13 to load stage 2 at memory 0x80000
- If disk read fails, jump to fail: → CPU halt

## Setup GDT
```bash 
gdt_start:
    dq 0x0000000000000000
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_end:

gdt_des:
    dw gdt_end - gdt_start - 1
    dd gdt_start
```
- 0x08 -> code_seg 0x10-> data_seg 
- use (lgdt [des]) for lode gdt

## Enter Protected Mode
```bash 
    mov eax,cr0
    or eax,1
    mov cr0,eax

    jmp 0x08:pm_mode_start
```
- Set PE bit in cr0 to enable protected mode
- Far jump to pm_mode_start (32-bit code)

## Protected Mode Initialization
```bash
mov ax,0x10
mov es,ax
mov ds,ax
mov fs,ax
mov gs,ax
mov ss,ax
mov esp,0x9FF00

jmp 0x08:0x80000
```
- Set 32-bit segment registers to code seg 
- Initialize stack for kernel execution
- Jump to kernel entry at 0x80000

## Fail
```bash
fail:
    hlt
    jmp $
```
- hlt stops the CPU until an interrupt occurs 
- jmp $ loops indefinitely to prevent undefined behavior
